#include "read_transfer_task.hpp"

int OSFFReadTransferTask::run()
{
    // pop packet from format packet queue
    AVPacket *pkt = nullptr;

    if ((this->status = this->pop_pkt_cb(this->osff_pkt_trans_ctx->fmt_pkt_queue_id,
                                         (void *&)pkt,
                                         THREAD_NO_TIMEOUT)) < 0)
    {
        av_log(nullptr, AV_LOG_ERROR,
               "Could not pop packet (error: '%s') for format packet queue #%d\n",
               err2str(this->status).c_str(),
               this->osff_pkt_trans_ctx->fmt_pkt_queue_id);
        return this->status;
    }

    // push packet to all stream packet queues according to the transfer way
    if (pkt == nullptr)
    {
        this->exit_flag = true;

        for (auto fmt_it = this->osff_pkt_trans_ctx->fmt2strm_way.begin();
             fmt_it != this->osff_pkt_trans_ctx->fmt2strm_way.end();
             fmt_it++)
        {
            for (int i = 0; i < fmt_it->second.size(); ++i)
            {
                int strm_queue_id = this->osff_pkt_trans_ctx->fmt2strm_queues_id[fmt_it->first][i];

                if ((this->status = this->push_pkt_cb(strm_queue_id,
                                                      nullptr,
                                                      THREAD_NO_TIMEOUT)) < 0)
                {
                    av_log(nullptr, AV_LOG_ERROR,
                           "Could not push packet (error: '%s') for stream packet queue #%d\n",
                           err2str(this->status).c_str(),
                           strm_queue_id);
                    return this->status;
                }
            }
        }
    }
    else
    {
        // find the corresponding stream packet queue and push the packet
        auto dstrm_it = this->osff_pkt_trans_ctx->fmt2strm_way.find(pkt->stream_index);

        if (dstrm_it != this->osff_pkt_trans_ctx->fmt2strm_way.end())
        {
            // push packet to stream packet queues
            for (int i = 0; i < dstrm_it->second.size(); ++i)
            {
                // clone packet
                int strm_queue_id = this->osff_pkt_trans_ctx->fmt2strm_queues_id[dstrm_it->first][i];
                int strm_id = dstrm_it->second[i];
                AVPacket *new_pkt = nullptr;

                if ((new_pkt = av_packet_clone(pkt)) == nullptr)
                {
                    av_log(nullptr, AV_LOG_ERROR,
                           "Could not clone packet for format #%d and stream #%d\n",
                           this->osff_pkt_trans_ctx->fmt_id,
                           strm_id);
                    av_packet_free(&pkt);
                    this->status = AVERROR(ENOMEM);
                    this->exit_flag = true;
                    return this->status;
                }

                // push packet to stream packet queue
                if ((this->status = this->push_pkt_cb(strm_queue_id,
                                                      pkt,
                                                      THREAD_NO_TIMEOUT)) < 0)
                {
                    av_log(nullptr, AV_LOG_ERROR,
                           "Could not push packet (error: '%s') for stream packet queue #%d\n",
                           err2str(this->status).c_str(),
                           strm_queue_id);
                    av_packet_free(&new_pkt);
                    this->exit_flag = true;
                    return this->status;
                }
            }
        }

        av_packet_free(&pkt);
    }
}