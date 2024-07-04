#ifndef SNAKE_EYE_READ_TASK_HPP
#define SNAKE_EYE_READ_TASK_HPP

#include <functional>

#include "base_task.hpp"
#include "../context/format_context.hpp"
#include "../util/util.hpp"

namespace SnakeEye
{
    class SnakeEyeReadTask : public SnakeEyeBaseTask
    {
    private:
        SnakeEyeFormatContext *se_fmt_ctx = nullptr;

        std::function<int(int, void *, int)> push_pkt_cb;

    public:
        SnakeEyeReadTask(SnakeEyeFormatContext *se_fmt_ctx,
                         std::function<int(int, void *, int)> push_pkt_cb)
            : se_fmt_ctx(se_fmt_ctx),
              push_pkt_cb(push_pkt_cb) {}

        virtual int run();
    };
}

#endif // SNAKE_EYE_READ_TASK_HPP
