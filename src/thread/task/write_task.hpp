#ifndef OSFF_WRITE_TASK_HPP
#define OSFF_WRITE_TASK_HPP

#include "base_task.hpp"
#include "../../context/format_context.hpp"

class OSFFWriteTask : public OSFFBaseTask
{
private:
    OSFFFormatContext *osff_fmt_ctx = nullptr;

    std::function<int(int, void *&, int)> pop_pkt_cb;

    bool is_write_header = false;

    int num_nullptr = 0;

public:
    OSFFWriteTask(OSFFFormatContext *osff_fmt_ctx,
                  std::function<int(int, void *&, int)> pop_pkt_cb)
        : osff_fmt_ctx(osff_fmt_ctx),
          pop_pkt_cb(pop_pkt_cb) {}

    virtual int run();

    int write_header();

    int write_tailer();
};

#endif // OSFF_WRITE_TASK_HPP