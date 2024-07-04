#ifndef SNAKE_EYE_READ_TRANSFER_TASK_HPP
#define SNAKE_EYE_READ_TRANSFER_TASK_HPP

#include <functional>

#include "base_task.hpp"
#include "../context/packet_transfer_context.hpp"

namespace SnakeEye
{
    class SnakeEyeReadTransferTask : public SnakeEyeBaseTask
    {
    private:
        SnakeEyePacketTransferContext *se_pkt_trans_ctx = nullptr;

        std::function<int(int, void *&, int)> pop_pkt_cb;

        std::function<int(int, void *, int)> push_pkt_cb;

    public:
        SnakeEyeReadTransferTask(SnakeEyePacketTransferContext *se_pkt_trans_ctx,
                                 std::function<int(int, void *&, int)> pop_pkt_cb,
                                 std::function<int(int, void *, int)> push_pkt_cb)
            : se_pkt_trans_ctx(se_pkt_trans_ctx),
              pop_pkt_cb(pop_pkt_cb),
              push_pkt_cb(push_pkt_cb) {}

        virtual int run();
    };
}

#endif // SNAKE_EYE_READ_TRANSFER_TASK_HPP
