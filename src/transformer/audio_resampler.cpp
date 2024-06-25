#include "audio_resampler.hpp"

OSFFAudioResampler::OSFFAudioResampler(int dstrm_idx,
                                       int estrm_idx,
                                       const AVCodecContext *dec_ctx,
                                       const AVCodecContext *enc_ctx)
    : OSFFTransformer(AVMEDIA_TYPE_AUDIO,
                      dstrm_idx,
                      estrm_idx,
                      dec_ctx,
                      enc_ctx)
{
    int error = 0;
    AVChannelLayout play_ch_layout;
    av_channel_layout_default(&play_ch_layout, 2);
    const AVChannelLayout *dec_ch_layout = &(dec_ctx->ch_layout);
    AVSampleFormat dec_smp_fmt = dec_ctx->sample_fmt;
    int dec_smp_rate = dec_ctx->sample_rate;
    const AVChannelLayout *enc_ch_layout = enc_ctx == nullptr ? &play_ch_layout : &(enc_ctx->ch_layout);
    AVSampleFormat enc_smp_fmt = enc_ctx == nullptr ? AV_SAMPLE_FMT_S16 : enc_ctx->sample_fmt;
    int enc_smp_rate = enc_ctx == nullptr ? dec_smp_rate : enc_ctx->sample_rate;

    this->dst_sample_rate = enc_ctx->sample_rate;

    // set options and allocate swr context
    if ((error = swr_alloc_set_opts2(&this->swr_ctx,
                                     enc_ch_layout,
                                     enc_smp_fmt,
                                     enc_smp_rate,
                                     dec_ch_layout,
                                     dec_smp_fmt,
                                     dec_smp_rate,
                                     0, NULL)) < 0)
    {
        av_log(NULL, AV_LOG_ERROR,
               "Could not allocate resampler context for %s (error: '%s')\n",
               this->strm_map_info.c_str(),
               err2str(error).c_str());
        this->status = error;
        return;
    }

    // initialize swr context
    if ((error = swr_init(this->swr_ctx)) < 0)
    {
        av_log(this->swr_ctx, AV_LOG_ERROR,
               "Could not initialize the resampler context for %s (error: '%s')\n",
               this->strm_map_info.c_str(),
               err2str(error).c_str());
        this->status = error;
        return;
    }

    av_log(this->swr_ctx, AV_LOG_INFO,
           "Initialized the resampler context for %s\n",
           this->strm_map_info.c_str());
}

int OSFFAudioResampler::send_frame(AVFrame *dec_frm)
{
    int error = 0;

    // check if decoding frame is nullptr
    bool flush = (dec_frm == nullptr);

    if (flush)
    {
        // push nullptr to indicate that there is no more frame
        this->frm_cache.push(nullptr);
        av_log(NULL, AV_LOG_INFO,
               "Audio resampler for decoding stream #%d and encoding stream #%d reached EOF\n",
               this->dstrm_idx,
               this->estrm_idx);
        return 0;
    }

    // send decoding audio frame to swr context
    if ((error = swr_convert_frame(this->swr_ctx,
                                   NULL,
                                   dec_frm)) < 0)
    {
        av_log(this->swr_ctx, AV_LOG_ERROR,
               "Could not convert audio frame for decoding stream #%d (error: '%s')\n",
               this->dstrm_idx,
               err2str(error).c_str());
        return error;
    }

    int remain_samples = 0;
    int start_pts = dec_frm->pts;
    int start_dts = dec_frm->pkt_dts;
    int start_best_eff_pts = dec_frm->best_effort_timestamp;

    // get transformed samples from swr context
    while ((remain_samples = swr_get_out_samples(this->swr_ctx, 0)) > 0)
    {
        // allocate a new encoding frame
        AVFrame *new_enc_frm = nullptr;

        if (this->enc_ctx == nullptr)
        {
            AVChannelLayout audio_play_ch_layout;
            av_channel_layout_default(&audio_play_ch_layout, 2);

            if ((error = init_audio_frame(new_enc_frm,
                                          remain_samples,
                                          AV_SAMPLE_FMT_S16,
                                          audio_play_ch_layout,
                                          this->dec_ctx->sample_rate)) < 0)
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

        // get encoding audio frame from swr context
        if ((error = swr_convert_frame(this->swr_ctx,
                                       new_enc_frm,
                                       NULL)) < 0)
        {
            av_log(this->swr_ctx, AV_LOG_ERROR,
                   "Could not convert audio frame for %s (error: '%s')\n",
                   this->strm_map_info.c_str(),
                   err2str(error).c_str());
            av_frame_free(&new_enc_frm);
            return error;
        }
        else
        {
            // set pts, duration and dts
            int64_t duration = calc_duration(new_enc_frm->nb_samples);
            new_enc_frm->duration = duration;
            new_enc_frm->pts = start_pts;
            new_enc_frm->pkt_dts = start_dts;
            new_enc_frm->best_effort_timestamp = start_best_eff_pts;
            start_pts += duration;
            start_dts += duration;
            start_best_eff_pts += duration;
            this->frm_cache.push(new_enc_frm);
        }
    }

    return error;
}
