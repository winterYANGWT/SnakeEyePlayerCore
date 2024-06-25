#include "codec_context_manager.h"

CodecContextManager::~CodecContextManager()
{
    for (auto it = this->dec_thread_ctxs.begin();
         it != this->dec_thread_ctxs.end();
         it++)
    {
        delete it->second;
    }

    for (auto it = this->enc_thread_ctxs.begin();
         it != this->enc_thread_ctxs.end();
         it++)
    {
        delete it->second;
    }

    this->dec_thread_ctxs.clear();
    this->enc_thread_ctxs.clear();
    this->ctxs_ref_count.clear();
}

int CodecContextManager::add_encoder_context(int enc_strm_idx,
                                             int dec_strm_idx,
                                             AVStream *dec_strm)
{
    int error = 0;

    // check if encoder context for stream_idx already exists
    auto it = this->ctxs_ref_count.find(enc_strm_idx);

    if (it == this->ctxs_ref_count.end() ||
        it->second == 0)
    {
        // get codec
        const AVCodec *codec = nullptr;

        if ((codec = avcodec_find_encoder(dec_strm->codecpar->codec_id)) == nullptr)
        {
            av_log(NULL, AV_LOG_ERROR,
                   "Could not find encoder for stream #%d\n",
                   enc_strm_idx);
            return AVERROR_ENCODER_NOT_FOUND;
        }
        else
        {
            av_log(NULL, AV_LOG_INFO,
                   "Found encoder %s for stream #%d\n",
                   avcodec_get_name(dec_strm->codecpar->codec_id),
                   enc_strm_idx);
        }

        // allocate codec context
        AVCodecContext *codec_ctx = nullptr;

        if ((codec_ctx = avcodec_alloc_context3(codec)) == nullptr)
        {
            av_log(NULL, AV_LOG_ERROR,
                   "Could not allocate an encoder context for stream #%d\n",
                   enc_strm_idx);
            return AVERROR(ENOMEM);
        }
        else
        {
            av_log(NULL, AV_LOG_INFO,
                   "Allocated an encoder context for stream #%d\n",
                   enc_strm_idx);
        }

        // copy codec parameters from stream to codec context
        if ((error = avcodec_parameters_to_context(codec_ctx,
                                                   dec_strm->codecpar)) < 0)
        {
            av_log(codec_ctx, AV_LOG_ERROR,
                   "Could not initialize an encoder context from decoding stream #%d (error: '%s')\n",
                   dec_strm_idx,
                   err2str(error).c_str());
            return error;
        }
        else
        {
            av_log(codec_ctx, AV_LOG_INFO,
                   "Initialized an encoder context from decoding stream #%d\n",
                   dec_strm_idx);
        }

        // set encoder timebase
        codec_ctx->time_base = dec_strm->time_base;

        // open codec
        if ((error = avcodec_open2(codec_ctx,
                                   codec,
                                   NULL)) < 0)
        {
            avcodec_free_context(&codec_ctx);
            av_log(codec_ctx, AV_LOG_ERROR,
                   "Could not open encoder for stream #%d (error: '%s')\n",
                   enc_strm_idx,
                   err2str(error).c_str());
            return error;
        }
        else
        {
            av_log(codec_ctx, AV_LOG_INFO,
                   "Opened encoder for stream #%d\n",
                   enc_strm_idx);
            this->enc_thread_ctxs[enc_strm_idx] = new OSFFEncodeThreadContext(enc_strm_idx,
                                                                              codec_ctx,
                                                                              this->pop_frm_cb_func,
                                                                              this->push_pkt_cb_func);
        }

        this->ctxs_ref_count[enc_strm_idx] = 1;
    }
    else
    {
        this->ctxs_ref_count[enc_strm_idx]++;
    }

    return error;
}

int CodecContextManager::get_encoder_context(int strm_idx,
                                             AVCodecContext *&enc_ctx)
{
    auto it = this->enc_thread_ctxs.find(strm_idx);

    if (it == this->enc_thread_ctxs.end() ||
        it->second == nullptr)
    {
        av_log(NULL, AV_LOG_ERROR,
               "Encoder context for stream #%d does not exist\n",
               strm_idx);
        return AVERROR(EINVAL);
    }

    it->second->get_context(enc_ctx);
    return 0;
}

int CodecContextManager::delete_encoder_context(int enc_strm_idx)
{
    auto it = this->ctxs_ref_count.find(enc_strm_idx);

    if (it == this->ctxs_ref_count.end() ||
        it->second == 0)
    {
        av_log(NULL, AV_LOG_ERROR,
               "Encoder context for stream #%d does not exist\n",
               enc_strm_idx);
        return AVERROR(EINVAL);
    }
    else
    {
        this->ctxs_ref_count[enc_strm_idx]--;

        if (this->ctxs_ref_count[enc_strm_idx] == 0)
        {
            delete this->enc_thread_ctxs[enc_strm_idx];
            this->enc_thread_ctxs[enc_strm_idx] = nullptr;
        }
    }

    return 0;
}

int CodecContextManager::add_decoder_context(int strm_idx,
                                             AVStream *strm)
{
    int error = 0;

    // check if decoder context for stream_idx already exists
    auto it = this->ctxs_ref_count.find(strm_idx);

    if (it == this->ctxs_ref_count.end() ||
        it->second == 0)
    {
        // get codec
        const AVCodec *codec = nullptr;

        if ((codec = avcodec_find_decoder(strm->codecpar->codec_id)) == nullptr)
        {
            av_log(NULL, AV_LOG_ERROR,
                   "Could not find decoder for stream #%d\n",
                   strm_idx);
            return AVERROR_DECODER_NOT_FOUND;
        }
        else
        {
            av_log(NULL, AV_LOG_INFO,
                   "Found decoder %s for stream #%d\n",
                   avcodec_get_name(strm->codecpar->codec_id),
                   strm_idx);
        }

        // allocate codec context
        AVCodecContext *codec_ctx = nullptr;

        if ((codec_ctx = avcodec_alloc_context3(codec)) == nullptr)
        {
            av_log(NULL, AV_LOG_ERROR,
                   "Could not allocate a decoder context for stream #%d\n",
                   strm_idx);
            return AVERROR(ENOMEM);
        }
        else
        {
            av_log(NULL, AV_LOG_INFO,
                   "Allocated a decoder context for stream #%d\n",
                   strm_idx);
        }

        // copy codec parameters from stream to codec context
        if ((error = avcodec_parameters_to_context(codec_ctx,
                                                   strm->codecpar)) < 0)
        {
            av_log(codec_ctx, AV_LOG_ERROR,
                   "Could not initialize a decoder context from stream #%d (error: '%s')\n",
                   strm_idx,
                   err2str(error).c_str());
            return error;
        }
        else
        {
            av_log(codec_ctx, AV_LOG_INFO,
                   "Initialized decoder a context from stream #%d\n",
                   strm_idx);
        }

        // set time base for codec context
        // decoder time base is the same as stream time base so that pts, dts and duration can not be converted
        codec_ctx->time_base = strm->time_base;

        // open codec
        if ((error = avcodec_open2(codec_ctx,
                                   codec,
                                   NULL)) < 0)
        {
            avcodec_free_context(&codec_ctx);
            av_log(codec_ctx, AV_LOG_ERROR,
                   "Could not open decoder for stream #%d (error: '%s')\n",
                   strm_idx,
                   err2str(error).c_str());
            return error;
        }
        else
        {
            av_log(codec_ctx, AV_LOG_INFO,
                   "Opened decoder for stream #%d\n",
                   strm_idx);
            this->dec_thread_ctxs[strm_idx] = new OSFFDecodeThreadContext(strm_idx,
                                                                          codec_ctx,
                                                                          this->pop_pkt_cb_func,
                                                                          this->push_frm_cb_func);
        }

        this->ctxs_ref_count[strm_idx] = 1;
    }
    else
    {
        this->ctxs_ref_count[strm_idx]++;
    }

    return error;
}

int CodecContextManager::get_decoder_context(int strm_idx,
                                             AVCodecContext *&dec_ctx)
{
    auto it = this->dec_thread_ctxs.find(strm_idx);

    if (it == this->dec_thread_ctxs.end() ||
        it->second == nullptr)
    {
        av_log(NULL, AV_LOG_ERROR,
               "Decoder context for stream #%d does not exist\n",
               strm_idx);
        return AVERROR(EINVAL);
    }

    it->second->get_context(dec_ctx);
    return 0;
}

int CodecContextManager::delete_decoder_context(int strm_idx)
{
    auto it = this->ctxs_ref_count.find(strm_idx);

    if (it == this->ctxs_ref_count.end() ||
        it->second == 0)
    {
        av_log(NULL, AV_LOG_ERROR,
               "Decoder context for stream #%d does not exist\n",
               strm_idx);
        return AVERROR(EINVAL);
    }
    else
    {
        this->ctxs_ref_count[strm_idx]--;

        if (this->ctxs_ref_count[strm_idx] == 0)
        {
            delete this->dec_thread_ctxs[strm_idx];
            this->dec_thread_ctxs[strm_idx] = nullptr;
        }
    }

    return 0;
}

int CodecContextManager::start()
{
    // start all decode threads
    for (auto it = this->dec_thread_ctxs.begin();
         it != this->dec_thread_ctxs.end();
         it++)
    {
        it->second->start();
    }

    // start all encode threads
    for (auto it = this->enc_thread_ctxs.begin();
         it != this->enc_thread_ctxs.end();
         it++)
    {
        it->second->start();
    }

    return 0;
}