#ifndef SNAKE_EYE_PLAY_FRAME_QUEUE_HPP
#define SNAKE_EYE_PLAY_FRAME_QUEUE_HPP

extern "C"
{
#include <libavutil/frame.h>
}

#include "../element/play_frame.hpp"
#include "data_queue.hpp"

namespace SnakeEye
{
    class SnakeEyePlayFrameQueue : public SnakeEyeDataQueue
    {
    private:
        AVRational strm_tb = {1, AV_TIME_BASE};

        double d_strm_tb = 0.0;

        int cur_size = 0;

    private:
        int reset_time(SnakeEyePlayFrame *frm);

    public:
        const static std::string type;

    public:
        SnakeEyePlayFrameQueue(AVRational strm_tb,
                               int max_size)
            : strm_tb(strm_tb),
              SnakeEyeDataQueue(max_size)
        {
            this->d_strm_tb = av_q2d(strm_tb);
        }

        virtual int preprocess(void *data)
        {
            if (data != nullptr)
            {
                this->reset_time((SnakeEyePlayFrame *)data);
            }

            return 0;
        }

        virtual int destroy_data(void *data)
        {
            if (data != nullptr)
            {
                SnakeEyePlayFrame *frm = (SnakeEyePlayFrame *)data;
                delete frm;
            }

            return 0;
        }

        int peek_last_frame(SnakeEyePlayFrame *&frm);

        int peek_frame(SnakeEyePlayFrame *&frm);

        int peek_next_frame(SnakeEyePlayFrame *&frm);
    };

    const std::string SnakeEyePlayFrameQueue::type = "SnakeEyePlayFrameQueue";
}

#endif // SNAKE_EYE_PLAY_FRAME_QUEUE_HPP
