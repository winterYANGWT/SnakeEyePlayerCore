#include "util.hpp"

int SnakeEye::dump_format_info(int fmt_ctx_idx,
                               AVFormatContext *fmt_ctx,
                               nlohmann::json &info)
{
    info.clear();
    char str_buf[1024] = {0};

    // set general info
    if (fmt_ctx->iformat)
    {
        info["type"] = "input";
        info["format_name"] = fmt_ctx->iformat->name;
    }
    else if (fmt_ctx->oformat)
    {
        info["type"] = "output";
        info["format_name"] = fmt_ctx->oformat->name;
    }
    else
    {
        info["type"] = "unknown";
        info["format_name"] = "N/A";
    }

    info["num_streams"] = std::to_string(fmt_ctx->nb_streams);
    info["url"] = fmt_ctx->url;
    info["bit_rate"] = std::to_string(fmt_ctx->bit_rate);

    // set time related info
    snprintf(str_buf, 1024, "%d/%d", 1, AV_TIME_BASE);
    info["time_base"] = str_buf;
    info["start_time"] = std::to_string(av_q2d(AV_TIME_BASE_Q) * fmt_ctx->start_time);
    info["duration"] = std::to_string(av_q2d(AV_TIME_BASE_Q) * fmt_ctx->duration);

    return 0;
}

int SnakeEye::dump_stream_info(int strm_idx,
                               AVStream *strm,
                               nlohmann::json &info)
{
    info.clear();
    char str_buf[1024] = {0};

    // set general info
    info["stream_index"] = std::to_string(strm_idx);
    info["in_format_stream_index"] = std::to_string(strm->index);

    // set time related info
    snprintf(str_buf, 1024, "%d/%d", strm->time_base.num, strm->time_base.den);
    info["time_base"] = str_buf;

    if (strm->duration != AV_NOPTS_VALUE)
    {
        info["duration"] = std::to_string(av_q2d(strm->time_base) * strm->duration);
    }
    else
    {
        info["duration"] = "N/A";
    }

    if (strm->start_time != AV_NOPTS_VALUE)
    {
        info["start_time"] = std::to_string(av_q2d(strm->time_base) * strm->start_time);
    }
    else
    {
        info["start_time"] = "N/A";
    }

    info["num_frames"] = std::to_string(strm->nb_frames);

    // set codec related info
    const AVCodecDescriptor *codec_desc = avcodec_descriptor_get(strm->codecpar->codec_id);
    info["codec_name"] = codec_desc->long_name;
    info["bit_rate"] = std::to_string(strm->codecpar->bit_rate);

    if (strm->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
    {
        info["codec_type"] = "video";
        info["pixel_format"] = av_get_pix_fmt_name((AVPixelFormat)strm->codecpar->format);
        snprintf(str_buf, 1024, "%d:%d", strm->codecpar->width, strm->codecpar->height);
        info["width:height"] = str_buf;
        snprintf(str_buf, 1024, "%d/%d", strm->avg_frame_rate.num, strm->avg_frame_rate.den);
        info["frame_rate"] = str_buf;
    }
    else if (strm->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
    {
        info["codec_type"] = "audio";
        info["sample_format"] = av_get_sample_fmt_name((AVSampleFormat)strm->codecpar->format);
        info["num_channels"] = std::to_string(strm->codecpar->ch_layout.nb_channels);
        info["sample_rate"] = std::to_string(strm->codecpar->sample_rate);
    }

    return 0;
}
