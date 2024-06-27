#ifndef SNAKE_EYE_DECODE_TASK_HPP
#define SNAKE_EYE_DECODE_TASK_HPP

#include "base_task.hpp"
#include "../../context/stream_context.hpp"

namespace SnakeEye
{
    class SnakeEyeDecodeTask : public SnakeEyeBaseTask
    {
    private:
        SnakeEyeStreamContext *se_strm_ctx = nullptr;

        std::function<int(int, void *&, int)> pop_pkt_cb;

        std::function<int(int, void *, int)> push_frm_cb;

    public:
        SnakeEyeDecodeTask(SnakeEyeStreamContext *se_strm_ctx,
                           std::function<int(int, void *&, int)> pop_pkt_cb,
                           std::function<int(int, void *, int)> push_frm_cb)
            : se_strm_ctx(se_strm_ctx),
              pop_pkt_cb(pop_pkt_cb),
              push_frm_cb(push_frm_cb) {}

        virtual int run();
    };
}

#endif // SNAKE_EYE_DECODE_TASK_HPP
