#ifndef TRANSFORMER_MANAGER_H
#define TRANSFORMER_MANAGER_H

#include "../element/transformer.h"
#include "../element/audio_resampler.h"
#include "../element/video_scaler.h"
#include "../thread/transform_thread.h"

#include <vector>
#include <unordered_map>
#include <functional>

/**
 * @class TransformerManager
 * @brief Class responsible for managing transformers.
 */
class TransformerManager
{
private:
    std::unordered_map<int, OSFFTransformThreadContext *> trfms;

    std::function<int(int, AVFrame *&, int)> pop_dec_strm_frm_cb_func;

    std::function<int(int, AVFrame *, int)> push_enc_strm_frm_cb_func;

private:
    int add_audio_resampler(int dec_strm_idx,
                            int enc_strm_idx,
                            AVCodecContext *dec_ctx,
                            AVCodecContext *enc_ctx);

    int add_video_scaler(int dec_strm_idx,
                         int enc_strm_idx,
                         AVCodecContext *dec_ctx,
                         AVCodecContext *enc_ctx);

public:
    TransformerManager(std::function<int(int, AVFrame *&, int)> pop_dec_strm_frm_cb_func,
                       std::function<int(int, AVFrame *, int)> push_enc_strm_frm_cb_func)
        : pop_dec_strm_frm_cb_func(pop_dec_strm_frm_cb_func),
          push_enc_strm_frm_cb_func(push_enc_strm_frm_cb_func) {}

    ~TransformerManager();

    int add_transformer(int dec_strm_idx,
                        int enc_strm_idx,
                        AVCodecContext *dec_ctx,
                        AVCodecContext *enc_ctx);

    int add_audio_play_transformer(int dec_strm_idx,
                                   AVCodecContext *dec_ctx);

    int add_video_play_transformer(int dec_strm_idx,
                                   AVCodecContext *dec_ctx);

    int delete_transformer(int dec_strm_idx,
                           int enc_strm_idx)
    {
        int error = 0;
        auto it = this->trfms.find(dec_strm_idx);

        if (it == this->trfms.end())
        {
            av_log(nullptr, AV_LOG_ERROR,
                   "Could not delete transformer for decoding stream #%d and encoding stream #%d\n",
                   dec_strm_idx,
                   enc_strm_idx);
            return AVERROR(EINVAL);
        }

        if ((error = it->second->delete_transformer(enc_strm_idx)) < 0)
        {
            av_log(nullptr, AV_LOG_ERROR,
                   "Could not delete transformer for decoding stream #%d and encoding stream #%d\n",
                   dec_strm_idx,
                   enc_strm_idx);
            return error;
        }

        return 0;
    }

    int get_transformer(int dec_strm_idx,
                        int enc_strm_idx,
                        Transformer *&transformer)
    {
        int error = 0;
        auto it = this->trfms.find(dec_strm_idx);

        if (it == this->trfms.end())
        {
            av_log(nullptr, AV_LOG_ERROR,
                   "No transformer for decoding stream #%d and encoding stream #%d\n",
                   dec_strm_idx,
                   enc_strm_idx);
            return AVERROR(EINVAL);
        }

        if ((error = it->second->get_transformer(enc_strm_idx,
                                                 transformer)) < 0)
        {
            av_log(nullptr, AV_LOG_ERROR,
                   "No transformer for decoding stream #%d and encoding stream #%d\n",
                   dec_strm_idx,
                   enc_strm_idx);
            return error;
        }

        return 0;
    }

    int start()
    {
        int error = 0;

        for (auto it = this->trfms.begin();
             it != this->trfms.end();
             it++)
        {
            if ((error = it->second->start()) < 0)
            {
                return error;
            }
        }

        return 0;
    }
};

#endif // TRANSFORMER_MANAGER_H
