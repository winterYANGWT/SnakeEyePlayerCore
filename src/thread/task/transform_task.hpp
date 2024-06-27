#ifndef SNAKE_EYE_TRANSFORMER_TASK_HPP
#define SNAKE_EYE_TRANSFORMER_TASK_HPP

#include "base_task.hpp"
#include "../../context/transformer_context.hpp"

namespace SnakeEye
{
    class SnakeEyeTransformerTask : public SnakeEyeBaseTask
    {
    private:
        SnakeEyeTransformerContext *se_trfm_ctx = nullptr;

        std::function<int(int, void *&, int)> pop_frm_cb;

        std::function<int(int, void *, int)> push_frm_cb;

    public:
        SnakeEyeTransformerTask(SnakeEyeTransformerContext *se_trfm_ctx,
                                std::function<int(int, void *&, int)> pop_frm_cb,
                                std::function<int(int, void *, int)> push_frm_cb)
            : se_trfm_ctx(se_trfm_ctx),
              pop_frm_cb(pop_frm_cb),
              push_frm_cb(push_frm_cb) {}

        virtual int run();
    };
}

#endif // SNAKE_EYE_TRANSFORMER_TASK_HPP
