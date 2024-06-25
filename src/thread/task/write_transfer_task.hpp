#ifndef OSFF_TRANSFER_TASK_HPP
#define OSFF_TRANSFER_TASK_HPP

#include <functional>
#include <list>

#include "base_task.hpp"
#include "../../context/packet_transfer_context.hpp"

class OSFFTransferTask : public OSFFBaseTask
{
private:
    OSFFPacketTransferContext *osff_pkt_trans_ctx = nullptr;

    std::function<int(int, void *&, int)> pop_pkt_cb;

    std::function<int(int, void *, int)> push_pkt_cb;

    std::list<int> estrm_id_list;

public:
    OSFFTransferTask(OSFFPacketTransferContext *osff_pkt_trans_ctx,
                     std::function<int(int, void *&, int)> pop_pkt_cb,
                     std::function<int(int, void *, int)> push_pkt_cb)
        : osff_pkt_trans_ctx(osff_pkt_trans_ctx),
          pop_pkt_cb(pop_pkt_cb),
          push_pkt_cb(push_pkt_cb)
    {
        for (auto it = osff_pkt_trans_ctx->strm2fmt_way.begin();
             it != osff_pkt_trans_ctx->strm2fmt_way.end();
             it++)
        {
            estrm_id_list.push_back(it->first);
        }
    }

    virtual int run();
};

#endif // OSFF_TRANSFER_TASK_HPP