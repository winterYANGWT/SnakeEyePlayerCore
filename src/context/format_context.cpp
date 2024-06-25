#include "format_context.hpp"

int OSFFFormatContext::add_stream(AVCodecContext *codec_ctx,
                                  int &ostrm_id)
{
    // add a new stream to the output format context
    int error = 0;
    AVStream *strm = nullptr;

    if ((strm = avformat_new_stream(this->fmt_ctx,
                                    codec_ctx->codec)) == nullptr)
    {
        av_log(this->fmt_ctx, AV_LOG_ERROR,
               "Could not allocate stream for format context #%d\n",
               this->fmt_id);
        return AVERROR(ENOMEM);
    }

    // set codec flag
    if (this->fmt_ctx->oformat->flags & AVFMT_GLOBALHEADER)
    {
        codec_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
    }

    // copy codec context parameters to stream
    if ((error = avcodec_parameters_from_context(strm->codecpar,
                                                 codec_ctx)) < 0)
    {
        av_log(this->fmt_ctx, AV_LOG_ERROR,
               "Could not copy codec parameters to stream for format context #%d (error: '%s')\n",
               this->fmt_id,
               err2str(error));
        return error;
    }

    // set time base and initialize format context
    strm->time_base = codec_ctx->time_base;
    strm->codecpar->codec_tag = 0;

    if ((error = avformat_init_output(this->fmt_ctx,
                                      nullptr)) < 0)
    {
        av_log(this->fmt_ctx, AV_LOG_ERROR,
               "Could not initialize output format context #%d (error: '%s')\n",
               this->fmt_id,
               err2str(error));
        return error;
    }

    ++this->num_strms;
    ostrm_id = this->num_strms - 1;
    return error;
}