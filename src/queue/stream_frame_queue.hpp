#ifndef OSFF_STREAM_FRAME_QUEUE_HPP
#define OSFF_STREAM_FRAME_QUEUE_HPP

extern "C"
{
#include <libavutil/frame.h>
}

#include "data_queue.hpp"

class OSFFStreamFrameQueue : public OSFFDataQueue
{
private:
    AVRational base_tb = {1, AV_TIME_BASE};

    AVRational prev_tb = {1, AV_TIME_BASE};

    AVRational cur_tb = {1, AV_TIME_BASE};

private:
    int reset_time(AVFrame *frm);

public:
    const static std::string type;

public:
    OSFFStreamFrameQueue(AVRational &prev_tb,
                         AVRational &cur_tb,
                         int max_size = -1)
        : prev_tb(prev_tb),
          cur_tb(cur_tb),
          OSFFDataQueue(max_size) {}

    virtual int preprocess(void *data)
    {
        if (data != nullptr)
        {
            this->reset_time((AVFrame *)data);
        }

        return 0;
    }

    virtual int destroy_data(void *data)
    {
        if (data != nullptr)
        {
            av_frame_free((AVFrame **)&data);
        }

        return 0;
    }
};

const std::string OSFFStreamFrameQueue::type = "OSFFStreamFrameQueue";

#endif // OSFF_STREAM_FRAME_QUEUE_HPP
