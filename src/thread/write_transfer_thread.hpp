#ifndef SNAKE_EYE_WRITE_TRANSFER_THREAD_HPP
#define SNAKE_EYE_WRITE_TRANSFER_THREAD_HPP

#include "base_thread.hpp"
#include "../task/write_transfer_task.hpp"

namespace SnakeEye
{
    class SnakeEyeWriteTransferThread : public SnakeEyeBaseThread
    {
    private:
        SnakeEyePacketTransferContext *se_pkt_trans_ctx = nullptr;

        std::function<int(int, void *&, int)> pop_pkt_cb;

        std::function<int(int, void *, int)> push_pkt_cb;

    private:
        virtual SnakeEyeBaseTask *get_task()
        {
            return new SnakeEyeWriteTransferTask(this->se_pkt_trans_ctx,
                                                 this->pop_pkt_cb,
                                                 this->push_pkt_cb);
        }

    public:
        SnakeEyeWriteTransferThread(SnakeEyePacketTransferContext *se_pkt_trans_ctx,
                                    std::function<int(int, void *&, int)> pop_pkt_cb,
                                    std::function<int(int, void *, int)> push_pkt_cb)
            : se_pkt_trans_ctx(se_pkt_trans_ctx),
              pop_pkt_cb(pop_pkt_cb),
              push_pkt_cb(push_pkt_cb) {}
    };
}

#endif // SNAKE_EYE_WRITE_TRANSFER_THREAD_HPP
