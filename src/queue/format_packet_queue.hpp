#ifndef OSFF_FORMAT_PACKET_QUEUE_HPP
#define OSFF_FORMAT_PACKET_QUEUE_HPP

extern "C"
{
#include <libavformat/avformat.h>
}
#include <vector>

#include "../util/util.h"
#include "data_queue.hpp"

class OSFFFormatPacketQueue : public OSFFDataQueue
{
private:
    int64_t start_time = 0;

    AVRational base_tb = {1, AV_TIME_BASE};

    std::vector<AVRational> streams_tb;

private:
    int reset_time(AVPacket *pkt);

public:
    const static std::string type;

public:
    OSFFFormatPacketQueue(AVFormatContext *fmt_ctx,
                          int max_size = -1);

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

const std::string OSFFFormatPacketQueue::type = "OSFFFormatPacketQueue";

#endif // OSFF_FORMAT_PACKET_QUEUE_HPP
