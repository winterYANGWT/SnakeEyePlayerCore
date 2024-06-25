#include "transformer_manager.h"

int TransformerManager::add_audio_resampler(int dec_strm_idx,
                                            int enc_strm_idx,
                                            AVCodecContext *dec_ctx,
                                            AVCodecContext *enc_ctx)
{
    int error = 0;
    AudioResampler *audio_resampler = new AudioResampler(dec_strm_idx,
                                                         enc_strm_idx,
                                                         dec_ctx,
                                                         enc_ctx);

    if ((error = audio_resampler->get_status()) < 0)
    {

        delete audio_resampler;
    }
    else
    {
        auto it = this->trfms.find(dec_strm_idx);

        if (it == this->trfms.end())
        {
            this->trfms[dec_strm_idx] = new OSFFTransformThreadContext(dec_strm_idx,
                                                                       this->pop_dec_strm_frm_cb_func,
                                                                       this->push_enc_strm_frm_cb_func);
        }

        if ((error = this->trfms[dec_strm_idx]->add_transformer(enc_strm_idx,
                                                                audio_resampler)) < 0)
        {
            delete audio_resampler;
            av_log(NULL, AV_LOG_ERROR,
                   "Audio resampler for decoding stream #%d and encoding stream #%d already exists\n",
                   dec_strm_idx,
                   enc_strm_idx);
            return error;
        }
    }

    return error;
}

int TransformerManager::add_video_scaler(int dec_strm_idx,
                                         int enc_strm_idx,
                                         AVCodecContext *dec_ctx,
                                         AVCodecContext *enc_ctx)
{
    int error = 0;
    VideoScaler *video_scaler = new VideoScaler(dec_strm_idx,
                                                enc_strm_idx,
                                                dec_ctx,
                                                enc_ctx);

    if ((error = video_scaler->get_status()) < 0)
    {
        delete video_scaler;
    }
    else
    {
        auto it = this->trfms.find(dec_strm_idx);

        if (it == this->trfms.end())
        {
            this->trfms[dec_strm_idx] = new OSFFTransformThreadContext(dec_strm_idx,
                                                                       this->pop_dec_strm_frm_cb_func,
                                                                       this->push_enc_strm_frm_cb_func);
        }

        if ((error = this->trfms[dec_strm_idx]->add_transformer(enc_strm_idx,
                                                                video_scaler)) < 0)
        {
            delete video_scaler;
            av_log(NULL, AV_LOG_ERROR,
                   "Video scaler for decoding stream #%d and encoding stream #%d already exists\n",
                   dec_strm_idx,
                   enc_strm_idx);
            return error;
        }
    }

    return error;
}

TransformerManager::~TransformerManager()
{
    for (auto trfm_it = this->trfms.begin();
         trfm_it != this->trfms.end();
         trfm_it++)
    {
        delete trfm_it->second;
    }

    this->trfms.clear();
}

int TransformerManager::add_transformer(int dec_strm_idx,
                                        int enc_strm_idx,
                                        AVCodecContext *dec_ctx,
                                        AVCodecContext *enc_ctx)
{
    if (dec_strm_idx < 0 || enc_strm_idx < 0)
    {
        av_log(NULL, AV_LOG_ERROR,
               "Stream index should be non-negative\n");
        return AVERROR(EINVAL);
    }

    if (dec_ctx->codec_type != enc_ctx->codec_type)
    {
        av_log(NULL, AV_LOG_ERROR,
               "The media type of decoder and encoder should be the same\n");
        return AVERROR(EINVAL);
    }

    // add transformer according to media type
    enum AVMediaType media_type = dec_ctx->codec_type;

    if (media_type == AVMEDIA_TYPE_AUDIO)
    {
        return this->add_audio_resampler(dec_strm_idx,
                                         enc_strm_idx,
                                         dec_ctx,
                                         enc_ctx);
    }
    else if (media_type == AVMEDIA_TYPE_VIDEO)
    {
        return this->add_video_scaler(dec_strm_idx,
                                      enc_strm_idx,
                                      dec_ctx,
                                      enc_ctx);
    }
    else
    {
        av_log(NULL, AV_LOG_ERROR,
               "Unsupported media type\n");
        return AVERROR(EINVAL);
    }
}

int TransformerManager::add_audio_play_transformer(int dec_strm_idx,
                                                   AVCodecContext *dec_ctx)
{
    int error = 0;
    AudioResampler *audio_resampler = new AudioResampler(dec_strm_idx,
                                                         dec_ctx);

    if ((error = audio_resampler->get_status()) < 0)
    {
        delete audio_resampler;
    }
    else
    {
        auto it = this->trfms.find(dec_strm_idx);

        if (it == this->trfms.end())
        {
            this->trfms[dec_strm_idx] = new OSFFTransformThreadContext(dec_strm_idx,
                                                                       this->pop_dec_strm_frm_cb_func,
                                                                       this->push_enc_strm_frm_cb_func);
        }

        if ((error = this->trfms[dec_strm_idx]->add_transformer(0x10000 - 1,
                                                                audio_resampler)) < 0)
        {
            delete audio_resampler;
            av_log(NULL, AV_LOG_ERROR,
                   "Audio resampler for decoding stream #%d and encoding stream #%d already exists\n",
                   dec_strm_idx,
                   0x10000 - 2);
            return error;
        }
    }

    return error;
}

int TransformerManager::add_video_play_transformer(int dec_strm_idx, AVCodecContext *dec_ctx)
{
    int error = 0;

    VideoScaler *video_scaler = new VideoScaler(dec_strm_idx,
                                                dec_ctx);

    if ((error = video_scaler->get_status()) < 0)
    {
        delete video_scaler;
    }
    else
    {
        auto it = this->trfms.find(dec_strm_idx);

        if (it == this->trfms.end())
        {
            this->trfms[dec_strm_idx] = new OSFFTransformThreadContext(dec_strm_idx,
                                                                       this->pop_dec_strm_frm_cb_func,
                                                                       this->push_enc_strm_frm_cb_func);
        }

        if ((error = this->trfms[dec_strm_idx]->add_transformer(0x10000 - 2,
                                                                video_scaler)) < 0)
        {
            delete video_scaler;
            av_log(NULL, AV_LOG_ERROR,
                   "Video scaler for decoding stream #%d and encoding stream #%d already exists\n",
                   dec_strm_idx,
                   0x10000 - 2);
            return error;
        }
    }

    return error;
}
