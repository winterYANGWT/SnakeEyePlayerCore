#include "read_task.hpp"

int SnakeEye::SnakeEyeReadTask::run()
{
    // allocate a packet
    AVPacket *pkt = nullptr;

    if (this->status = init_packet(&pkt) < 0)
    {
        this->exit_flag = true;
        return this->status;
    }

    // read a packet from format context
    if (this->status = av_read_frame(this->se_fmt_ctx->fmt_ctx,
                                     pkt) < 0)
    {
        // error occurs, free the packet
        av_packet_free(&pkt);

        // if reaches the end of file, push nullptr to queue and set exit flag to true, then return
        // if the data is not available at the moment, set error to status and return
        // otherwise, set exit flag to true and return
        if (this->status == AVERROR_EOF)
        {
            av_log(this->se_fmt_ctx->fmt_ctx, AV_LOG_INFO,
                   "Reached the end of file for format #%d\n",
                   this->se_fmt_ctx->fmt_id);
            this->exit_flag = true;
            this->status = 0;
        }
        else if (this->status == AVERROR(EAGAIN))
        {
            this->status = 0;
            return this->status;
        }
        else
        {
            av_log(this->se_fmt_ctx->fmt_ctx, AV_LOG_ERROR,
                   "Could not read frame (error: '%s') for format #%d\n",
                   err2str(this->status).c_str(),
                   this->se_fmt_ctx->fmt_id);
            this->exit_flag = true;
            return this->status;
        }
    }

    // use callback function to push a packet to queue
    if (this->status = this->push_pkt_cb(this->se_fmt_ctx->fmt_pkt_queue_id,
                                         pkt,
                                         THREAD_NO_TIMEOUT) < 0)
    {
        av_packet_free(&pkt);
        av_log(nullptr, AV_LOG_ERROR,
               "Could not push packet (error: '%s') for format_packet_queue #%d\n",
               err2str(this->status).c_str(),
               this->se_fmt_ctx->fmt_pkt_queue_id);
        this->exit_flag = true;
    }

    return this->status;
}
