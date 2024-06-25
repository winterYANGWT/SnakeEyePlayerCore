#ifndef OSFF_READ_TRANSFER_TASK_HPP
#define OSFF_READ_TRANSFER_TASK_HPP

#include <functional>

#include "base_task.hpp"
#include "../../context/packet_transfer_context.hpp"

class OSFFReadTransferTask : public OSFFBaseTask
{
private:
    OSFFPacketTransferContext *osff_pkt_trans_ctx = nullptr;

    std::function<int(int, void *&, int)> pop_pkt_cb;

    std::function<int(int, void *, int)> push_pkt_cb;

public:
    OSFFReadTransferTask(OSFFPacketTransferContext *osff_pkt_trans_ctx,
                         std::function<int(int, void *&, int)> pop_pkt_cb,
                         std::function<int(int, void *, int)> push_pkt_cb)
        : osff_pkt_trans_ctx(osff_pkt_trans_ctx),
          pop_pkt_cb(pop_pkt_cb),
          push_pkt_cb(push_pkt_cb) {}

    virtual int run();
};

#endif // OSFF_READ_TRANSFER_TASK_HPP