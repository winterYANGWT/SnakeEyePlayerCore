#include "transform_task.hpp"

int OSFFTransformerTask::run()
{
    AVFrame *dec_frm = nullptr;
    AVFrame *enc_frm = nullptr;

    if ((this->status = this->pop_frm_cb(this->osff_trfm_ctx->dstrm_idx,
                                         (void *&)dec_frm,
                                         THREAD_NO_TIMEOUT)) < 0)
    {
        av_log(nullptr, AV_LOG_ERROR,
               "Could not pop frame (error: '%s') for transform thread (decode stream index #%d)\n",
               err2str(this->status).c_str(),
               this->osff_trfm_ctx->dstrm_idx);
        this->exit_flag = true;
        return this->status;
    }

    if (dec_frm == nullptr)
    {
        this->exit_flag = true;
    }

    for (auto it = this->osff_trfm_ctx->trfms.begin();
         it != this->osff_trfm_ctx->trfms.end();
         ++it)
    {
        int estrm_idx = it->first;
        OSFFTransformer *trfm = it->second;

        // send the frame to transformer
        if ((this->status = trfm->send_frame(dec_frm)) < 0)
        {
            av_frame_free(&dec_frm);
            this->exit_flag = true;
            return this->status;
        }

        while (true)
        {
            // receive the frame from transformer
            enc_frm = nullptr;

            if ((this->status = trfm->receive_frame(enc_frm)) < 0)
            {
                if (this->status == AVERROR(EAGAIN))
                {
                    this->status = 0;
                    break;
                }
            }
            else
            {
                // use callback function to push the frame to encode stream frame queue
                if ((this->status = this->push_frm_cb(estrm_idx,
                                                      enc_frm,
                                                      THREAD_NO_TIMEOUT)) < 0)
                {
                    av_log(nullptr, AV_LOG_ERROR,
                           "Could not push frame (error: '%s') for transform thread (decode stream index #%d, encode stream index #%d)\n",
                           err2str(this->status).c_str(),
                           this->osff_trfm_ctx->dstrm_idx,
                           estrm_idx);
                    av_frame_free(&enc_frm);
                    this->exit_flag = true;
                    return this->status;
                }
            }
        }
    }

    return this->status;
}