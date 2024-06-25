#ifndef OSFF_AUDIO_RESAMPLER_HPP
#define OSFF_AUDIO_RESAMPLER_HPP

extern "C"
{
#include <libswresample/swresample.h>
}

#include "transformer.hpp"

class OSFFAudioResampler : public OSFFTransformer
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
    OSFFAudioResampler(int dstrm_idx,
                       int estrm_idx,
                       const AVCodecContext *dec_ctx,
                       const AVCodecContext *enc_ctx);

    ~OSFFAudioResampler()
    {
        swr_free(&this->swr_ctx);
    };

    virtual int send_frame(AVFrame *dec_frm);
};

#endif // OSFF_AUDIO_RESAMPLER_HPP
