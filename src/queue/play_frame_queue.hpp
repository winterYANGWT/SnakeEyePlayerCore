#ifndef OSFF_PLAY_FRAME_QUEUE_HPP
#define OSFF_PLAY_FRAME_QUEUE_HPP

extern "C"
{
#include <libavutil/frame.h>
}

#include "../element/play_frame.h"
#include "data_queue.hpp"

class OSFFPlayFrameQueue : public OSFFDataQueue
{
private:
    AVRational strm_tb = {1, AV_TIME_BASE};

    double d_strm_tb = 0.0;

    int cur_size = 0;

private:
    int reset_time(PlayFrame *frm);

public:
    const static std::string type;

public:
    OSFFPlayFrameQueue(AVRational strm_tb,
                       int max_size)
        : strm_tb(strm_tb),
          OSFFDataQueue(max_size)
    {
        this->d_strm_tb = av_q2d(strm_tb);
    }

    virtual int preprocess(void *data)
    {
        if (data != nullptr)
        {
            this->reset_time((PlayFrame *)data);
        }

        return 0;
    }

    virtual int destroy_data(void *data)
    {
        if (data != nullptr)
        {
            PlayFrame *frm = (PlayFrame *)data;
            delete frm;
        }

        return 0;
    }

    int peek_last_frame(PlayFrame *&frm);

    int peek_frame(PlayFrame *&frm);

    int peek_next_frame(PlayFrame *&frm);
};

const std::string OSFFPlayFrameQueue::type = "PlayFrameQueue";

#endif // OSFF_PLAY_FRAME_QUEUE_HPP