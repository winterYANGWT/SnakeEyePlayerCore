#include "util.hpp"

int SnakeEye::init_frame(AVFrame **frm)
{
    if ((*frm = av_frame_alloc()) == nullptr)
    {
        av_log(NULL, AV_LOG_ERROR,
               "Could not allocate frame\n");
        return AVERROR(ENOMEM);
    }

    return 0;
}

int SnakeEye::init_frame_with_codec_ctx(AVFrame **frm,
                                        const AVCodecContext *codec_ctx)
{
    if (codec_ctx->codec_type == AVMEDIA_TYPE_VIDEO)
    {
        return init_video_frame(*frm,
                                codec_ctx->width,
                                codec_ctx->height,
                                codec_ctx->pix_fmt);
    }
    else if (codec_ctx->codec_type == AVMEDIA_TYPE_AUDIO)
    {
        return init_audio_frame(*frm,
                                codec_ctx->frame_size,
                                codec_ctx->sample_fmt,
                                codec_ctx->ch_layout,
                                codec_ctx->sample_rate);
    }

    return AVERROR(EINVAL);
}

int SnakeEye::init_audio_frame(AVFrame *&frm,
                               int nb_samples,
                               AVSampleFormat sample_fmt,
                               AVChannelLayout channel_layout,
                               int sample_rate)
{
    int error = 0;

    if ((frm = av_frame_alloc()) == nullptr)
    {
        av_log(NULL, AV_LOG_ERROR,
               "Could not allocate frame\n");
        return AVERROR(ENOMEM);
    }

    frm->nb_samples = nb_samples;
    frm->format = sample_fmt;
    av_channel_layout_copy(&frm->ch_layout,
                           &channel_layout);
    frm->sample_rate = sample_rate;

    if ((error = av_frame_get_buffer(frm, 0)) < 0)
    {
        av_log(frm, AV_LOG_ERROR,
               "Could not allocate frame buffers (error: '%s')\n",
               err2str(error).c_str());
        av_frame_free(&frm);
    }

    return error;
}

int SnakeEye::init_video_frame(AVFrame *&frm,
                               int width,
                               int height,
                               AVPixelFormat pix_fmt)
{
    int error = 0;

    if ((frm = av_frame_alloc()) == nullptr)
    {
        av_log(NULL, AV_LOG_ERROR,
               "Could not allocate frame\n");
        return AVERROR(ENOMEM);
    }

    frm->width = width;
    frm->height = height;
    frm->format = pix_fmt;

    if ((error = av_frame_get_buffer(frm, 32)) < 0)
    {
        av_log(frm, AV_LOG_ERROR,
               "Could not allocate frame buffers (error: '%s')\n",
               err2str(error).c_str());
        av_frame_free(&frm);
    }

    return error;
}

int SnakeEye::init_packet(AVPacket **pkt)
{
    if ((*pkt = av_packet_alloc()) == nullptr)
    {
        av_log(NULL, AV_LOG_ERROR,
               "Could not allocate packet\n");
        return AVERROR(ENOMEM);
    }

    return 0;
}
