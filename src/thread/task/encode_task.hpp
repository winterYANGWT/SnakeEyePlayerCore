#ifndef OSFF_ENCODE_TASK_HPP
#define OSFF_ENCODE_TASK_HPP

#include "base_task.hpp"
#include "../../context/stream_context.hpp"

class OSFFEncodeTask : public OSFFBaseTask
{
private:
    OSFFStreamContext *osff_strm_ctx = nullptr;

    std::function<int(int, void *&, int)> pop_frm_cb;

    std::function<int(int, void *, int)> push_pkt_cb;

public:
    OSFFEncodeTask(OSFFStreamContext *osff_strm_ctx,
                   std::function<int(int, void *&, int)> pop_frm_cb,
                   std::function<int(int, void *, int)> push_pkt_cb)
        : osff_strm_ctx(osff_strm_ctx),
          pop_frm_cb(pop_frm_cb),
          push_pkt_cb(push_pkt_cb) {}

    virtual int run();
};

#endif // OSFF_ENCODE_TASK_HPP