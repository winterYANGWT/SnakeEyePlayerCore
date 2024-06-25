#ifndef OSFF_READ_TRANSFER_THREAD_HPP
#define OSFF_READ_TRANSFER_THREAD_HPP

#include <functional>

#include "base_thread.hpp"
#include "../task/read_transfer_task.hpp"

class OSFFReadTransferThread : public OSFFBaseThread
{
private:
    OSFFFormatContext *osff_fmt_ctx = nullptr;

    std::function<int(int, void *&, int)> pop_pkt_cb;

    std::function<int(int, void *, int)> push_pkt_cb;

private:
    virtual OSFFBaseTask *get_task()
    {
        return new OSFFReadTransferTask(this->osff_fmt_ctx,
                                        this->pop_pkt_cb,
                                        this->push_pkt_cb);
    }

public:
    OSFFReadTransferThread(OSFFFormatContext *osff_fmt_ctx,
                           std::function<int(int, void *&, int)> pop_pkt_cb,
                           std::function<int(int, void *, int)> push_pkt_cb)
        : osff_fmt_ctx(osff_fmt_ctx),
          pop_pkt_cb(pop_pkt_cb),
          push_pkt_cb(push_pkt_cb) {}
};

#endif // OSFF_READ_TRANSFER_THREAD_HPP