#ifndef SNAKE_EYE_ENCODE_TASK_HPP
#define SNAKE_EYE_ENCODE_TASK_HPP

#include "base_task.hpp"
#include "../context/stream_context.hpp"

namespace SnakeEye
{
    class SnakeEyeEncodeTask : public SnakeEyeBaseTask
    {
    private:
        SnakeEyeStreamContext *se_strm_ctx = nullptr;

        std::function<int(int, void *&, int)> pop_frm_cb;

        std::function<int(int, void *, int)> push_pkt_cb;

    public:
        SnakeEyeEncodeTask(SnakeEyeStreamContext *se_strm_ctx,
                           std::function<int(int, void *&, int)> pop_frm_cb,
                           std::function<int(int, void *, int)> push_pkt_cb)
            : se_strm_ctx(se_strm_ctx),
              pop_frm_cb(pop_frm_cb),
              push_pkt_cb(push_pkt_cb) {}

        virtual int run();
    };
}

#endif // SNAKE_EYE_ENCODE_TASK_HPP
