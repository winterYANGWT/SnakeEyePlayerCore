#include "format_packet_queue.hpp"

int SnakeEye::SnakeEyeFormatPacketQueue::reset_time(AVPacket *pkt)
{
    if (this->start_time != 0)
    {
        if (pkt->pts != AV_NOPTS_VALUE)
        {
            int64_t new_pts = time_sub(pkt->pts,
                                       this->streams_tb[pkt->stream_index],
                                       this->start_time,
                                       this->base_tb);
            new_pts = av_rescale_q_rnd(new_pts,
                                       this->base_tb,
                                       this->streams_tb[pkt->stream_index],
                                       AV_ROUND_NEAR_INF);
            pkt->pts = new_pts;
        }

        if (pkt->dts != AV_NOPTS_VALUE)
        {
            int64_t new_dts = time_sub(pkt->dts,
                                       this->streams_tb[pkt->stream_index],
                                       this->start_time,
                                       this->base_tb);
            new_dts = av_rescale_q_rnd(new_dts,
                                       this->base_tb,
                                       this->streams_tb[pkt->stream_index],
                                       AV_ROUND_NEAR_INF);
            pkt->dts = new_dts;
        }
    }

    return 0;
}

SnakeEye::SnakeEyeFormatPacketQueue::SnakeEyeFormatPacketQueue(AVFormatContext *fmt_ctx,
                                                               int max_size)
    : SnakeEyeDataQueue(max_size)
{
    this->start_time = fmt_ctx->start_time;

    for (int i = 0; i < fmt_ctx->nb_streams; i++)
    {
        this->streams_tb.push_back(fmt_ctx->streams[i]->time_base);
    }
}
