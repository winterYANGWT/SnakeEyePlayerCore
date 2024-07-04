#include "write_task.hpp"

int SnakeEye::SnakeEyeWriteTask::run()
{
    if (!this->is_write_header)
    {
        if ((this->status = this->write_header()) < 0)
        {
            return this->status;
        }

        this->is_write_header = true;
    }

    AVPacket *pkt = nullptr;

    if ((this->status = this->pop_pkt_cb(this->se_fmt_ctx->fmt_pkt_queue_id,
                                         (void *&)pkt,
                                         THREAD_NO_TIMEOUT)) < 0)
    {
        av_log(this->se_fmt_ctx->fmt_ctx, AV_LOG_ERROR,
               "Could not pop packet (error: '%s') for format_packet_queue #%d\n",
               err2str(this->status).c_str(),
               this->se_fmt_ctx->fmt_pkt_queue_id);
        this->exit_flag = true;
        return this->status;
    }

    // write packet to format context
    // if error occurs, free the packet and set exit flag to true
    if ((this->status = av_interleaved_write_frame(this->se_fmt_ctx->fmt_ctx,
                                                   pkt)) < 0)
    {
        av_log(this->se_fmt_ctx->fmt_ctx, AV_LOG_ERROR,
               "Could not write frame (error: '%s') for format #%d\n",
               err2str(this->status).c_str(),
               this->se_fmt_ctx->fmt_id);
        av_packet_free(&pkt);
        this->exit_flag = true;
        return this->status;
    }

    if (pkt == nullptr)
    {
        ++this->num_nullptr;

        if (this->num_nullptr == this->se_fmt_ctx->fmt_ctx->nb_streams)
        {
            this->status = this->write_tailer();
            this->exit_flag = true;
        }
    }
    else
    {
        av_packet_free(&pkt);
    }

    return this->status;
}

int SnakeEye::SnakeEyeWriteTask::write_header()
{
    if (this->se_fmt_ctx->fmt_ctx->oformat->flags & AVFMT_NOFILE)
    {
        av_log(this->se_fmt_ctx->fmt_ctx, AV_LOG_ERROR,
               "No need for writing header for format #%d\n",
               this->se_fmt_ctx->fmt_id);
        return 0;
    }
    else
    {
        if ((this->status = avformat_write_header(this->se_fmt_ctx->fmt_ctx,
                                                  nullptr)) < 0)
        {
            av_log(this->se_fmt_ctx->fmt_ctx, AV_LOG_ERROR,
                   "Could not write header (error: '%s') for format #%d\n",
                   err2str(this->status).c_str(),
                   this->se_fmt_ctx->fmt_id);
            this->exit_flag = true;
        }
        else
        {
            av_log(this->se_fmt_ctx->fmt_ctx, AV_LOG_INFO,
                   "Written header for format context #%d\n",
                   this->se_fmt_ctx->fmt_id);
        }

        return this->status;
    }
}

int SnakeEye::SnakeEyeWriteTask::write_tailer()
{
    if (this->se_fmt_ctx->fmt_ctx->oformat->flags & AVFMT_NOFILE)
    {
        av_log(this->se_fmt_ctx->fmt_ctx, AV_LOG_ERROR,
               "No need for writing trailer for format #%d\n",
               this->se_fmt_ctx->fmt_id);
        return 0;
    }
    else
    {
        if ((this->status = av_write_trailer(this->se_fmt_ctx->fmt_ctx)) < 0)
        {
            av_log(this->se_fmt_ctx->fmt_ctx, AV_LOG_ERROR,
                   "Could not write trailer (error: '%s') for format #%d\n",
                   err2str(this->status).c_str(),
                   this->se_fmt_ctx->fmt_id);
            this->exit_flag = true;
        }
        else
        {
            av_log(this->se_fmt_ctx->fmt_ctx, AV_LOG_INFO,
                   "Written trailer for format context #%d\n",
                   this->se_fmt_ctx->fmt_id);
        }

        return this->status;
    }
}
