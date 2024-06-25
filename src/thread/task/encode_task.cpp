#include "encode_task.hpp"

int OSFFEncodeTask::run()
{
    // pop a frame from the stream frame queue
    AVFrame *frm = nullptr;

    if ((this->status = this->pop_frm_cb(this->osff_strm_ctx->strm_frm_queue_id,
                                         (void *&)frm,
                                         THREAD_NO_TIMEOUT)) < 0)
    {
        av_log(nullptr, AV_LOG_ERROR,
               "Could not pop frame (error: '%s') for stream_frame_queue #%d\n",
               err2str(this->status).c_str(),
               this->osff_strm_ctx->strm_frm_queue_id);
        this->exit_flag = true;
        return this->status;
    }

    // send frame to encoder
    if ((this->status = avcodec_send_frame(this->osff_strm_ctx->codec_ctx,
                                           frm)) < 0)
    {
        av_log(this->osff_strm_ctx->codec_ctx, AV_LOG_ERROR,
               "Could not send frame (error: '%s') for stream #%d\n",
               err2str(this->status).c_str(),
               this->osff_strm_ctx->strm_id);
        av_frame_free(&frm);
        this->exit_flag = true;
        return this->status;
    }

    av_frame_free(&frm);

    while (!this->exit_flag)
    {
        AVPacket *pkt = nullptr;

        if ((this->status = init_packet(&pkt)) < 0)
        {
            this->exit_flag = true;
            return this->status;
        }

        // receive packet from codec
        if ((this->status = avcodec_receive_packet(this->osff_strm_ctx->codec_ctx,
                                                   pkt)) < 0)
        {
            // error occurs, free packet
            av_packet_free(&pkt);

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
                av_log(this->osff_strm_ctx->codec_ctx, AV_LOG_INFO,
                       "Reached the end of file for stream #%d\n",
                       this->osff_strm_ctx->strm_id);
                this->exit_flag = true;
            }
            else
            {
                av_log(this->osff_strm_ctx->codec_ctx, AV_LOG_ERROR,
                       "Could not receive packet (error: '%s') for stream #%d\n",
                       err2str(this->status).c_str(),
                       this->osff_strm_ctx->strm_id);
                this->exit_flag = true;
                return this->status;
            }
        }

        // push packet to the stream packet queue
        if ((this->status = this->push_pkt_cb(this->osff_strm_ctx->strm_pkt_queue_id,
                                              pkt,
                                              THREAD_NO_TIMEOUT)) < 0)
        {
            av_log(nullptr, AV_LOG_ERROR,
                   "Could not push packet (error: '%s') to stream_packet_queue #%d\n",
                   err2str(this->status).c_str(),
                   this->osff_strm_ctx->strm_pkt_queue_id);
            av_packet_free(&pkt);
            this->exit_flag = true;
            return this->status;
        }
    }

    return this->status;
}