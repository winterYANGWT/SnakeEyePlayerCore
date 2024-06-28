#include "write_transfer_task.hpp"

int SnakeEye::SnakeEyeWriteTransferTask::run()
{
    auto it = this->estrm_id_list.begin();

    while (it != this->estrm_id_list.end())
    {
        int estrm_id = *it;
        int ostrm_id = this->se_pkt_trans_ctx->strm2fmt_way[estrm_id];
        int estrm_queue_id = this->se_pkt_trans_ctx->strm2fmt_queues_id[estrm_id];

        // try to pop packet from stream packet queue
        AVPacket *pkt = nullptr;

        if ((this->status = this->pop_pkt_cb(estrm_queue_id,
                                             (void *&)pkt,
                                             DEFAULT_THREAD_TIMEOUT_MICROSECOND)) < 0)
        {
            if (this->status == AVERROR(ETIMEDOUT))
            {
                this->status = 0;
                continue;
            }
            else
            {
                av_log(nullptr, AV_LOG_ERROR,
                       "Could not pop packet (error: '%s') for stream packet queue #%d\n",
                       err2str(this->status).c_str(),
                       estrm_queue_id);
                this->exit_flag = true;
                return this->status;
            }
        }

        if (pkt != nullptr)
        {
            pkt->stream_index = ostrm_id;
        }

        // push packet to format packet queue
        if ((this->status = this->push_pkt_cb(this->se_pkt_trans_ctx->fmt_id,
                                              pkt,
                                              THREAD_NO_TIMEOUT)) < 0)
        {
            av_log(nullptr, AV_LOG_ERROR,
                   "Could not push packet (error: '%s') for format packet queue #%d\n",
                   err2str(this->status).c_str(),
                   ostrm_id);
            av_packet_free(&pkt);
            this->exit_flag = true;
            return this->status;
        }

        if (pkt == nullptr)
        {
            this->estrm_id_list.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
