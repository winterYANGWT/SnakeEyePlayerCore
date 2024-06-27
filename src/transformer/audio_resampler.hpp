#ifndef SNAKE_EYE_AUDIO_RESAMPLER_HPP
#define SNAKE_EYE_AUDIO_RESAMPLER_HPP

extern "C"
{
#include <libswresample/swresample.h>
}

#include "transformer.hpp"

namespace SnakeEye
{
    class SnakeEyeAudioResampler : public SnakeEyeTransformer
    {
    private:
        SwrContext *swr_ctx = nullptr;

        int dst_sample_rate = -1;

        AVRational base_tb = {1, AV_TIME_BASE};

    private:
        int64_t calc_duration(int nb_samples)
        {
            double duration_in_base_tb = (double)nb_samples / this->dst_sample_rate * AV_TIME_BASE;
            int64_t duration = av_rescale_q(duration_in_base_tb,
                                            this->base_tb,
                                            this->dec_ctx->time_base);
            return duration;
        }

    public:
        SnakeEyeAudioResampler(int dstrm_idx,
                               int estrm_idx,
                               const AVCodecContext *dec_ctx,
                               const AVCodecContext *enc_ctx);

        ~SnakeEyeAudioResampler()
        {
            swr_free(&this->swr_ctx);
        };

        virtual int send_frame(AVFrame *dec_frm);
    };
}

#endif // SNAKE_EYE_AUDIO_RESAMPLER_HPP
