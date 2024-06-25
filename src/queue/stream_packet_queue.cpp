#include "stream_packet_queue.hpp"

int OSFFStreamPacketQueue::reset_time(AVPacket *pkt)
{
    // do nothing if packet is nullptr
    if (pkt == nullptr)
    {
        return 0;
    }

    // firstly, convert packet's timebase to cur_tb
    if (this->prev_tb.num != this->cur_tb.num ||
        this->prev_tb.den != this->cur_tb.den)
    {
        av_packet_rescale_ts(pkt,
                             this->prev_tb,
                             this->cur_tb);
    }

    return 0;
}