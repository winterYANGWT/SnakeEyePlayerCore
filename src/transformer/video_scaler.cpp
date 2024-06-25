#include "video_scaler.hpp"

OSFFVideoScaler::OSFFVideoScaler(int dstrm_idx,
                                 int estrm_idx,
                                 const AVCodecContext *dec_ctx,
                                 const AVCodecContext *enc_ctx)
    : OSFFTransformer(AVMEDIA_TYPE_VIDEO,
                      dstrm_idx,
                      estrm_idx,
                      dec_ctx,
                      enc_ctx)
{
    int error = 0;
    int dec_width = dec_ctx->width;
    int dec_height = dec_ctx->height;
    AVPixelFormat dec_pix_fmt = dec_ctx->pix_fmt;
    int enc_width = enc_ctx == nullptr ? dec_width : enc_ctx->width;
    int enc_height = enc_ctx == nullptr ? dec_height : enc_ctx->height;
    AVPixelFormat enc_pix_fmt = enc_ctx == nullptr ? AV_PIX_FMT_YUV420P : enc_ctx->pix_fmt;

    // set options and allocate sws context
    if ((this->sws_ctx = sws_getContext(dec_width,
                                        dec_width,
                                        dec_pix_fmt,
                                        enc_width,
                                        enc_height,
                                        enc_pix_fmt,
                                        SWS_BILINEAR,
                                        NULL, NULL, NULL)) == nullptr)
    {
        av_log(NULL, AV_LOG_ERROR,
               "Could not allocate scaler context for %s\n",
               this->strm_map_info.c_str());
        this->status = AVERROR(ENOMEM);
        return;
    }
}

int OSFFVideoScaler::send_frame(AVFrame *dec_frm)
{
    int error = 0;

    // check if decoding frame is nullptr
    bool flush = (dec_frm == nullptr);

    if (flush)
    {
        // push nullptr into frame cache
        this->frm_cache.push(nullptr);
        av_log(NULL, AV_LOG_INFO,
               "Video scaler for decoding stream #%d and encoding stream #%d reached EOF\n",
               this->dstrm_idx,
               this->estrm_idx);
        return 0;
    }

    // allocate an encoding video frame
    AVFrame *new_enc_frm = nullptr;

    if (this->enc_ctx == nullptr)
    {
        if ((error = init_video_frame(new_enc_frm,
                                      dec_frm->width,
                                      dec_frm->height,
                                      AV_PIX_FMT_YUV420P)) < 0)
        {
            return error;
        }
    }
    else
    {
        if ((error = init_frame_with_codec_ctx(&new_enc_frm,
                                               this->enc_ctx)) < 0)
        {
            return error;
        }
    }

    // transform the decoding video frame to encoding video frame
    if ((error = sws_scale_frame(this->sws_ctx,
                                 new_enc_frm,
                                 dec_frm)) < 0)
    {
        av_log(this->sws_ctx, AV_LOG_ERROR,
               "Could not convert video frame for %s (error: '%s')\n",
               this->strm_map_info.c_str(),
               err2str(error).c_str());
    }
    else
    {
        new_enc_frm->pts = dec_frm->pts;
        new_enc_frm->duration = dec_frm->duration;
        new_enc_frm->pkt_dts = dec_frm->pkt_dts;
        new_enc_frm->best_effort_timestamp = dec_frm->best_effort_timestamp;
        this->frm_cache.push(new_enc_frm);
    }

    return error;
}
