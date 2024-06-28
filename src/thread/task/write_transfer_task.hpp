#ifndef SNAKE_EYE_TRANSFER_TASK_HPP
#define SNAKE_EYE_TRANSFER_TASK_HPP

#include <functional>
#include <list>

#include "base_task.hpp"
#include "../../context/packet_transfer_context.hpp"

namespace SnakeEye
{
    class SnakeEyeWriteTransferTask : public SnakeEyeBaseTask
    {
    private:
        SnakeEyePacketTransferContext *se_pkt_trans_ctx = nullptr;

        std::function<int(int, void *&, int)> pop_pkt_cb;

        std::function<int(int, void *, int)> push_pkt_cb;

        std::list<int> estrm_id_list;

    public:
        SnakeEyeWriteTransferTask(SnakeEyePacketTransferContext *se_pkt_trans_ctx,
                                  std::function<int(int, void *&, int)> pop_pkt_cb,
                                  std::function<int(int, void *, int)> push_pkt_cb)
            : se_pkt_trans_ctx(se_pkt_trans_ctx),
              pop_pkt_cb(pop_pkt_cb),
              push_pkt_cb(push_pkt_cb)
        {
            for (auto it = se_pkt_trans_ctx->strm2fmt_way.begin();
                 it != se_pkt_trans_ctx->strm2fmt_way.end();
                 it++)
            {
                estrm_id_list.push_back(it->first);
            }
        }

        virtual int run();
    };
}

#endif // SNAKE_EYE_TRANSFER_TASK_HPP
