#ifndef SNAKE_EYE_STREAM_PACKET_QUEUE_HPP
#define SNAKE_EYE_STREAM_PACKET_QUEUE_HPP

extern "C"
{
#include <libavformat/avformat.h>
}

#include "../util/util.h"
#include "data_queue.hpp"

namespace SnakeEye
{
    class SnakeEyeStreamPacketQueue : public SnakeEyeDataQueue
    {
    private:
        AVRational base_tb = {1, AV_TIME_BASE};

        AVRational prev_tb = {1, AV_TIME_BASE};

        AVRational cur_tb = {1, AV_TIME_BASE};

    private:
        int reset_time(AVPacket *pkt);

    public:
        const static std::string type;

    public:
        SnakeEyeStreamPacketQueue(AVRational &prev_tb,
                                  AVRational &cur_tb,
                                  int max_size = -1)
            : prev_tb(prev_tb),
              cur_tb(cur_tb),
              SnakeEyeDataQueue(max_size) {}

        virtual int preprocess(void *data)
        {
            if (data != nullptr)
            {
                this->reset_time((AVPacket *)data);
            }

            return 0;
        }

        virtual int destroy_data(void *data)
        {
            if (data != nullptr)
            {
                av_packet_free((AVPacket **)&data);
            }

            return 0;
        }
    };

    const std::string SnakeEyeStreamPacketQueue::type = "SnakeEyeStreamPacketQueue";
}

#endif // SNAKE_EYE_STREAM_PACKET_QUEUE_H
