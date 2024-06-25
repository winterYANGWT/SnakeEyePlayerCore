#ifndef OSFF_READ_TASK_HPP
#define OSFF_READ_TASK_HPP

#include <functional>

#include "base_task.hpp"
#include "../../context/format_context.hpp"
#include "../../util/util.h"

class OSFFReadTask : public OSFFBaseTask
{
private:
    OSFFFormatContext *osff_fmt_ctx = nullptr;

    std::function<int(int, void *, int)> push_pkt_cb;

public:
    OSFFReadTask(OSFFFormatContext *osff_fmt_ctx,
                 std::function<int(int, void *, int)> push_pkt_cb)
        : osff_fmt_ctx(osff_fmt_ctx),
          push_pkt_cb(push_pkt_cb) {}

    virtual int run();
};

#endif // OSFF_READ_TASK_HPP