#ifndef OSFF_TRANSFORMER_TASK_HPP
#define OSFF_TRANSFORMER_TASK_HPP

#include "base_task.hpp"
#include "../../context/transformer_context.hpp"

class OSFFTransformerTask : public OSFFBaseTask
{
private:
    OSFFTransformerContext *osff_trfm_ctx = nullptr;

    std::function<int(int, void *&, int)> pop_frm_cb;

    std::function<int(int, void *, int)> push_frm_cb;

public:
    OSFFTransformerTask(OSFFTransformerContext *osff_trfm_ctx,
                        std::function<int(int, void *&, int)> pop_frm_cb,
                        std::function<int(int, void *, int)> push_frm_cb)
        : osff_trfm_ctx(osff_trfm_ctx),
          pop_frm_cb(pop_frm_cb),
          push_frm_cb(push_frm_cb) {}

    virtual int run();
};

#endif // OSFF_TRANSFORMER_TASK_HPP