#ifndef OSFF_WRITE_THREAD_HPP
#define OSFF_WRITE_THREAD_HPP

#include "base_thread.hpp"
#include "../task/write_task.hpp"

class OSFFWriteThreadContext : public OSFFBaseThread
{
private:
    OSFFFormatContext *osff_fmt_ctx = nullptr;

    std::function<int(int, void *&, int)> pop_pkt_cb;

private:
    virtual OSFFBaseTask *get_task()
    {
        return new OSFFWriteTask(this->osff_fmt_ctx,
                                 this->pop_pkt_cb);
    }

public:
    OSFFWriteThreadContext(OSFFFormatContext *osff_fmt_ctx,
                           std::function<int(int, void *&, int)> pop_pkt_cb)
        : osff_fmt_ctx(osff_fmt_ctx),
          pop_pkt_cb(pop_pkt_cb) {}
};

#endif // OSFF_WRITE_THREAD_HPP