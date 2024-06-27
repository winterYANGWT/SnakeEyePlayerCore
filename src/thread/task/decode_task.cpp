#include "decode_task.hpp"

int SnakeEye::SnakeEyeDecodeTask::run()
{
    // pop a packet from the stream packet queue
    AVPacket *pkt = nullptr;

    if ((this->status = this->pop_pkt_cb(this->se_strm_ctx->strm_pkt_queue_id,
                                         (void *&)pkt,
                                         THREAD_NO_TIMEOUT)) < 0)
    {
        av_log(nullptr, AV_LOG_ERROR,
               "Could not pop packet (error: '%s') for stream_packet_queue #%d\n",
               err2str(this->status).c_str(),
               this->se_strm_ctx->strm_pkt_queue_id);
        this->exit_flag = true;
        return this->status;
    }

    // send packet to decoder
    if ((this->status = avcodec_send_packet(this->se_strm_ctx->codec_ctx,
                                            pkt)) < 0)
    {
        av_log(this->se_strm_ctx->codec_ctx, AV_LOG_ERROR,
               "Could not send packet (error: '%s') for stream #%d\n",
               err2str(this->status).c_str(),
               this->se_strm_ctx->strm_id);
        av_packet_free(&pkt);
        this->exit_flag = true;
        return this->status;
    }

    av_packet_free(&pkt);

    while (!this->exit_flag)
    {
        AVFrame *frm = nullptr;

        if ((this->status = init_frame(&frm)) < 0)
        {
            this->exit_flag = true;
            return this->status;
        }

        // receive frame from decoder
        if ((this->status = avcodec_receive_frame(this->se_strm_ctx->codec_ctx,
                                                  frm)) < 0)
        {
            // error occurs, free frame
            av_frame_free(&frm);

            // if the data is not available at the moment, set error to 0 and break
            // if reaches the end of file, push nullptr to queue and set exit_flag to true
            // otherwise, return error
            if (this->status == AVERROR(EAGAIN))
            {
                this->status = 0;
                break;
            }
            else if (this->status == AVERROR_EOF)
            {
                av_log(this->se_strm_ctx->codec_ctx, AV_LOG_INFO,
                       "Reached the end of file for stream #%d\n",
                       this->se_strm_ctx->strm_id);
                this->exit_flag = true;
            }
            else
            {
                av_log(this->se_strm_ctx->codec_ctx, AV_LOG_ERROR,
                       "Could not receive frame (error: '%s') for stream #%d\n",
                       err2str(this->status).c_str(),
                       this->se_strm_ctx->strm_id);
                this->exit_flag = true;
                return this->status;
            }
        }

        // push frame to the stream frame queue
        if ((this->status = this->push_frm_cb(this->se_strm_ctx->strm_frm_queue_id,
                                              frm,
                                              THREAD_NO_TIMEOUT)) < 0)
        {
            av_log(nullptr, AV_LOG_ERROR,
                   "Could not push frame (error: '%s') for stream_frame_queue #%d\n",
                   err2str(this->status).c_str(),
                   this->se_strm_ctx->strm_frm_queue_id);
            this->exit_flag = true;
            return this->status;
        }
    }

    return this->status;
}
