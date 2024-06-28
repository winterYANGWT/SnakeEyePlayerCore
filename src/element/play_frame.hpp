#ifndef SNAKE_EYE_PLAY_FRAME_HPP
#define SNAKE_EYE_PLAY_FRAME_HPP

extern "C"
{
#include <libavutil/frame.h>
}

namespace SnakeEye
{
    class SnakeEyePlayFrame
    {
    public:
        AVFrame *frm = nullptr;

        double pts = 0.0;

        double duration = 0.0;

    public:
        SnakeEyePlayFrame(AVFrame *frm,
                          double pts,
                          double duration)
        {
            this->frm = frm;
            this->pts = pts;
            this->duration = duration;
        }

        ~SnakeEyePlayFrame()
        {
            if (this->frm)
            {
                av_frame_free(&this->frm);
                this->frm = nullptr;
            }
        }
    };
}

#endif // SNAKE_EYE_PLAY_FRAME_HPP
