#include "stream_frame_queue.hpp"

int SnakeEye::SnakeEyeStreamFrameQueue::reset_time(AVFrame *frm)
{
    // do nothing if frame is nullptr
    if (frm == nullptr)
    {
        return 0;
    }

    // convert frame's timebase to cur_tb
    if (this->prev_tb.num != this->cur_tb.num ||
        this->prev_tb.den != this->cur_tb.den)
    {
        if (frm->pts != AV_NOPTS_VALUE)
        {
            frm->pts = av_rescale_q_rnd(frm->pts,
                                        this->prev_tb,
                                        this->cur_tb,
                                        AV_ROUND_NEAR_INF);
        }

        if (frm->pkt_dts != AV_NOPTS_VALUE)
        {
            frm->pkt_dts = av_rescale_q_rnd(frm->pkt_dts,
                                            this->prev_tb,
                                            this->cur_tb,
                                            AV_ROUND_NEAR_INF);
        }

        if (frm->duration != 0)
        {
            frm->duration = av_rescale_q_rnd(frm->duration,
                                             this->prev_tb,
                                             this->cur_tb,
                                             AV_ROUND_NEAR_INF);
        }

        if (frm->best_effort_timestamp != AV_NOPTS_VALUE)
        {
            frm->best_effort_timestamp = av_rescale_q_rnd(frm->best_effort_timestamp,
                                                          this->prev_tb,
                                                          this->cur_tb,
                                                          AV_ROUND_NEAR_INF);
        }
    }

    return 0;
}
