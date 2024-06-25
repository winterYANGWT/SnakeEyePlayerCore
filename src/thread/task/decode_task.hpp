#ifndef OSFF_DECODE_TASK_HPP
#define OSFF_DECODE_TASK_HPP

#include "base_task.hpp"
#include "../../context/stream_context.hpp"

class OSFFDecodeTask : public OSFFBaseTask
{
private:
    OSFFStreamContext *osff_strm_ctx = nullptr;

    std::function<int(int, void *&, int)> pop_pkt_cb;

    std::function<int(int, void *, int)> push_frm_cb;

public:
    OSFFDecodeTask(OSFFStreamContext *osff_strm_ctx,
                   std::function<int(int, void *&, int)> pop_pkt_cb,
                   std::function<int(int, void *, int)> push_frm_cb)
        : osff_strm_ctx(osff_strm_ctx),
          pop_pkt_cb(pop_pkt_cb),
          push_frm_cb(push_frm_cb) {}

    virtual int run();
};

#endif // OSFF_DECODE_TASK_HPP