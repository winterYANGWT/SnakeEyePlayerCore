#ifndef OSFF_TRANSFORM_THREAD_HPP
#define OSFF_TRANSFORM_THREAD_HPP

#include "base_thread.hpp"
#include "../task/transform_task.hpp"

class OSFFTransformThreadContext : public OSFFBaseThread
{
private:
    OSFFTransformerContext *osff_trfm_ctx = nullptr;

    std::function<int(int, void *&, int)> pop_frm_cb;

    std::function<int(int, void *, int)> push_frm_cb;

private:
    virtual OSFFBaseTask *get_task()
    {
        return new OSFFTransformerTask(this->osff_trfm_ctx,
                                       this->pop_frm_cb,
                                       this->push_frm_cb);
    }

public:
    OSFFTransformThreadContext(OSFFTransformerContext *osff_trfm_ctx,
                               std::function<int(int, void *&, int)> pop_frm_cb,
                               std::function<int(int, void *, int)> push_frm_cb)
        : osff_trfm_ctx(osff_trfm_ctx),
          pop_frm_cb(pop_frm_cb),
          push_frm_cb(push_frm_cb) {}
};

#endif // OSFF_TRANSFORM_THREAD_HPP