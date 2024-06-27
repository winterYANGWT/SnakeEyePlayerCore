#ifndef SNAKE_EYE_READ_TRANSFER_THREAD_HPP
#define SNAKE_EYE_READ_TRANSFER_THREAD_HPP

#include <functional>

#include "base_thread.hpp"
#include "../../context/format_context.hpp"
#include "../task/read_transfer_task.hpp"

namespace SnakeEye
{
    class SnakeEyeReadTransferThread : public SnakeEyeBaseThread
    {
    private:
        SnakeEyePacketTransferContext *se_pkt_trans_ctx = nullptr;

        std::function<int(int, void *&, int)> pop_pkt_cb;

        std::function<int(int, void *, int)> push_pkt_cb;

    private:
        virtual SnakeEyeBaseTask *get_task()
        {
            return new SnakeEyeReadTransferTask(this->se_pkt_trans_ctx,
                                                this->pop_pkt_cb,
                                                this->push_pkt_cb);
        }

    public:
        SnakeEyeReadTransferThread(SnakeEyePacketTransferContext *se_pkt_trans_ctx,
                                   std::function<int(int, void *&, int)> pop_pkt_cb,
                                   std::function<int(int, void *, int)> push_pkt_cb)
            : se_pkt_trans_ctx(se_pkt_trans_ctx),
              pop_pkt_cb(pop_pkt_cb),
              push_pkt_cb(push_pkt_cb) {}
    };
}

#endif // SNAKE_EYE_READ_TRANSFER_THREAD_HPP
