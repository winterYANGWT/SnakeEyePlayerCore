#include "format_context_manager.hpp"

OSFFFormatContextManager::~OSFFFormatContextManager()
{
    for (auto it = this->osff_fmt_ctxs.begin();
         it != this->osff_fmt_ctxs.end();
         it++)
    {
        delete it->second;
        it->second = nullptr;
    }
}

int OSFFFormatContextManager::open_input(int fmt_id,
                                         std::string url,
                                         const AVInputFormat *fmt,
                                         AVDictionary **opts)
{
    int error = 0;

    // open input and allocate format context
    AVFormatContext *fmt_ctx = nullptr;

    if ((error = avformat_open_input(&fmt_ctx,
                                     url.c_str(),
                                     fmt,
                                     opts)) < 0)
    {
        av_log(nullptr, AV_LOG_ERROR,
               "Could not open input: '%s' (error: '%s')\n",
               url.c_str(),
               err2str(error).c_str());
        return error;
    }

    // get all streams from input file
    if ((error = avformat_find_stream_info(fmt_ctx,
                                           nullptr)) < 0)
    {
        avformat_free_context(fmt_ctx);
        av_log(nullptr, AV_LOG_ERROR,
               "Could not find stream(s) of '%s' (error: '%s')\n",
               url.c_str(),
               err2str(error).c_str());
        return error;
    }

    // check if fmt_id is existed
    auto it = this->osff_fmt_ctxs.find(fmt_id);

    if (it != this->osff_fmt_ctxs.end())
    {
        avformat_free_context(fmt_ctx);
        av_log(nullptr, AV_LOG_ERROR,
               "OSFF format context #%d already exists\n",
               fmt_id);
        return AVERROR(EEXIST);
    }
    else
    {
        OSFFFormatContext *osff_fmt_ctx = new OSFFFormatContext(fmt_id,
                                                                fmt_ctx,
                                                                "input",
                                                                url,
                                                                fmt == nullptr ? fmt->name : "",
                                                                *opts);
        this->osff_fmt_ctxs[fmt_id] = osff_fmt_ctx;
        av_log(nullptr, AV_LOG_INFO,
               "Opened input: '%s' for format id #%d and %d streams\n",
               url.c_str(),
               fmt_id,
               fmt_ctx->nb_streams);
        return 0;
    }
}

int OSFFFormatContextManager::open_output(int fmt_id,
                                          std::string url,
                                          const AVOutputFormat *fmt,
                                          AVDictionary **opts)
{
    int error = 0;
    AVFormatContext *fmt_ctx = nullptr;

    // open output and allocate format context
    if ((error = avformat_alloc_output_context2(&fmt_ctx,
                                                fmt,
                                                nullptr,
                                                url.c_str())) < 0)
    {
        av_log(nullptr, AV_LOG_ERROR,
               "Could not open output: '%s' (error: '%s')\n",
               err2str(error).c_str());
        return error;
    }

    // open io context and bind it to format context
    AVIOContext *io_ctx = nullptr;

    if ((error = avio_open2(&io_ctx,
                            url.c_str(),
                            AVIO_FLAG_WRITE,
                            nullptr,
                            opts)) < 0)
    {
        av_log(nullptr, AV_LOG_ERROR,
               "Could not open output: '%s' (error: '%s')\n",
               url.c_str(),
               err2str(error).c_str());
        avformat_free_context(fmt_ctx);
        return error;
    }

    fmt_ctx->pb = io_ctx;

    // check if fmt_id is existed
    auto it = this->osff_fmt_ctxs.find(fmt_id);

    if (it != this->osff_fmt_ctxs.end())
    {
        avformat_free_context(fmt_ctx);
        av_log(nullptr, AV_LOG_ERROR,
               "OSFF format context #%d already exists\n",
               fmt_id);
        return AVERROR(EEXIST);
    }
    else
    {
        OSFFFormatContext *osff_fmt_ctx = new OSFFFormatContext(fmt_id,
                                                                fmt_ctx,
                                                                "output",
                                                                url,
                                                                fmt->name,
                                                                *opts);
        this->osff_fmt_ctxs[fmt_id] = osff_fmt_ctx;
        av_log(nullptr, AV_LOG_INFO,
               "Opened output: '%s' for format id #%d\n",
               url.c_str(),
               fmt_id);
        return fmt_id;
    }
}

// int OSFFFormatContextManager::add_input_device(int fmt_ctx_idx,
//                                                const std::string &audio_device_name,
//                                                const std::string &video_device_name,
//                                                AVDictionary *options)
// {
//     int error = 0;
//     AVFormatContext *fmt_ctx = nullptr;
//     std::string device_type = "";
//
// // get device type and device name
// #ifdef _WIN32
//     device_type = "dshow";
// #elif __linux__
//     device_type = "alsa";
// #elif __APPLE__ && __MACH__
//     device_type = "avfoundation";
// #endif
//
//     std::string device_name = generate_device_name(device_type,
//                                                    audio_device_name,
//                                                    video_device_name);
//
//     if (device_name == "")
//     {
//         av_log(nullptr, AV_LOG_ERROR,
//                "Could not find %s device(s): '%s'\n",
//                device_type.c_str(),
//                device_name.c_str());
//         return AVERROR(EINVAL);
//     }
//     else
//     {
//         av_log(nullptr, AV_LOG_INFO,
//                "Used %s device(s): '%s'\n",
//                device_type.c_str(),
//                device_name.c_str());
//     }
//
//     // set a big probesize to avoid av_find_stream_info() stuck
//     av_dict_set(&options, "probesize", "100M", 0);
//
//     return this->open_input(fmt_ctx_idx,
//                             device_name.c_str(),
//                             av_find_input_format(device_type.c_str()),
//                             &options);
// }

int OSFFFormatContextManager::get_format_context(int fmt_id,
                                                 OSFFFormatContext *&fmt_ctx)
{
    auto it = this->osff_fmt_ctxs.find(fmt_id);

    if (it != this->osff_fmt_ctxs.end())
    {
        fmt_ctx = it->second;
        return 0;
    }
    else
    {
        fmt_ctx = nullptr;
        av_log(nullptr, AV_LOG_ERROR,
               "OSFF format context #%d does not exist\n",
               fmt_id);
        return AVERROR(EINVAL);
    }
}

int OSFFFormatContextManager::add_output_stream(int fmt_id,
                                                AVCodecContext *codec_ctx,
                                                int &ostrm_idx)
{
    int error = 0;
    OSFFFormatContext *fmt_ctx = nullptr;

    if ((error = this->get_format_context(fmt_id,
                                          fmt_ctx)) < 0)
    {
        ostrm_idx = -1;
        return error;
    }

    if (fmt_ctx->io_type == "input")
    {
        av_log(nullptr, AV_LOG_ERROR,
               "Output stream cannot be added to input format context #%d\n",
               fmt_id);
        return AVERROR(EINVAL);
    }

    error = fmt_ctx->add_stream(codec_ctx, ostrm_idx);
    return error;
}

int OSFFFormatContextManager::delete_output_stream(int fmt_id)
{
    int error = 0;
    OSFFFormatContext *fmt_ctx = nullptr;

    if ((error = this->get_format_context(fmt_id,
                                          fmt_ctx)) < 0)
    {
        return error;
    }

    if (fmt_ctx->io_type == "input")
    {
        av_log(nullptr, AV_LOG_ERROR,
               "Output stream cannot be deleted from input format context #%d\n",
               fmt_id);
        return AVERROR(EINVAL);
    }

    if (this->write_thread_ctxs.find(fmt_ctx_idx) == this->write_thread_ctxs.end())
    {
        av_log(nullptr, AV_LOG_ERROR,
               "Output format context #%d does not exist\n",
               fmt_ctx_idx);
        return AVERROR(EINVAL);
    }
    else
    {
        return this->write_thread_ctxs[fmt_ctx_idx]->delete_stream();
    }
}

int FormatContextManager::get_stream(int fmt_idx,
                                     int in_fmt_strm_idx,
                                     AVStream *&strm)
{
    int error = 0;
    AVFormatContext *fmt_ctx = nullptr;

    if ((error = this->get_format_context(fmt_ctx_idx,
                                          fmt_ctx)) < 0)
    {
        strm = nullptr;
        return error;
    }

    if (in_fmt_strm_idx < 0 || in_fmt_strm_idx >= fmt_ctx->nb_streams)
    {
        strm = nullptr;
        av_log(NULL, AV_LOG_ERROR,
               "Stream #%d (in format) does not exist for format context #%d\n",
               in_fmt_strm_idx,
               fmt_ctx_idx);
        return AVERROR_STREAM_NOT_FOUND;
    }
    else
    {
        strm = fmt_ctx->streams[in_fmt_strm_idx];
        return 0;
    }
}

int FormatContextManager::start()
{
    for (auto it = this->read_thread_ctxs.begin();
         it != this->read_thread_ctxs.end();
         it++)
    {
        it->second->start();
    }

    for (auto it = this->write_thread_ctxs.begin();
         it != this->write_thread_ctxs.end();
         it++)
    {
        it->second->start();
    }

    return 0;
}
