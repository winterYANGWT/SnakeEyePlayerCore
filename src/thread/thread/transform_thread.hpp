#ifndef SNAKE_EYE_TRANSFORM_THREAD_HPP
#define SNAKE_EYE_TRANSFORM_THREAD_HPP

#include "base_thread.hpp"
#include "../task/transform_task.hpp"

namespace SnakeEye
{
    class SnakeEyeTransformThreadContext : public SnakeEyeBaseThread
    {
    private:
        SnakeEyeTransformerContext *se_trfm_ctx = nullptr;

        std::function<int(int, void *&, int)> pop_frm_cb;

        std::function<int(int, void *, int)> push_frm_cb;

    private:
        virtual SnakeEyeBaseTask *get_task()
        {
            return new SnakeEyeTransformerTask(this->se_trfm_ctx,
                                               this->pop_frm_cb,
                                               this->push_frm_cb);
        }

    public:
        SnakeEyeTransformThreadContext(SnakeEyeTransformerContext *se_trfm_ctx,
                                       std::function<int(int, void *&, int)> pop_frm_cb,
                                       std::function<int(int, void *, int)> push_frm_cb)
            : se_trfm_ctx(se_trfm_ctx),
              pop_frm_cb(pop_frm_cb),
              push_frm_cb(push_frm_cb) {}
    };
}

#endif // SNAKE_EYE_TRANSFORM_THREAD_HPP
