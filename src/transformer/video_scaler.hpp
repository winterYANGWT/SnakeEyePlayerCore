#ifndef SNAKE_EYE_VIDEO_SCALER_HPP
#define SNAKE_EYE_VIDEO_SCALER_HPP

extern "C"
{
#include <libswscale/swscale.h>
}

#include "transformer.hpp"

namespace SnakeEye
{
    class SnakeEyeVideoScaler : public SnakeEyeTransformer
    {
    private:
        SwsContext *sws_ctx = nullptr;

    public:
        SnakeEyeVideoScaler(int dstrm_idx,
                            int estrm_idx,
                            const AVCodecContext *dec_ctx,
                            const AVCodecContext *enc_ctx);

        ~SnakeEyeVideoScaler()
        {
            sws_freeContext(this->sws_ctx);
        };

        virtual int send_frame(AVFrame *dec_frm);
    };
}

#endif // SNAKE_EYE_VIDEO_SCALER_HPP
