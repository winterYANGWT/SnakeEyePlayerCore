#ifndef OSFF_READ_THREAD_HPP
#define OSFF_READ_THREAD_HPP

#include "base_thread.hpp"
#include "../task/read_task.hpp"

class OSFFReadThread : public OSFFBaseThread
{
private:
    OSFFFormatContext *osff_fmt_ctx = nullptr;

    std::function<int(int, void *, int)> push_pkt_cb;

private:
    OSFFReadTask *get_task()
    {
        return new OSFFReadTask(this->osff_fmt_ctx, this->push_pkt_cb);
    }

public:
    OSFFReadThread(OSFFFormatContext *osff_fmt_ctx,
                   std::function<int(int, void *, int)> push_pkt_cb)
        : osff_fmt_ctx(osff_fmt_ctx),
          push_pkt_cb(push_pkt_cb) {}
};

#endif // OSFF_READ_THREAD_HPP