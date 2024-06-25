#ifndef PLAY_FRAME_H
#define PLAY_FRAME_H

extern "C"
{
#include <libavutil/frame.h>
}

class PlayFrame
{
public:
    AVFrame *frm = nullptr;

    double pts = 0.0;

    double duration = 0.0;

public:
    PlayFrame(AVFrame *frm,
              double pts,
              double duration)
    {
        this->frm = frm;
        this->pts = pts;
        this->duration = duration;
    }

    ~PlayFrame()
    {
        if (this->frm)
        {
            av_frame_free(&this->frm);
            this->frm = nullptr;
        }
    }
};

#endif // PLAY_FRAME_H
