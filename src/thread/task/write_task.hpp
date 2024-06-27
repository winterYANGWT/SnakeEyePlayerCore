#ifndef SNAKE_EYE_WRITE_TASK_HPP
#define SNAKE_EYE_WRITE_TASK_HPP

#include "base_task.hpp"
#include "../../context/format_context.hpp"

namespace SnakeEye
{
    class SnakeEyeWriteTask : public SnakeEyeBaseTask
    {
    private:
        SnakeEyeFormatContext *se_fmt_ctx = nullptr;

        std::function<int(int, void *&, int)> pop_pkt_cb;

        bool is_write_header = false;

        int num_nullptr = 0;

    public:
        SnakeEyeWriteTask(SnakeEyeFormatContext *se_fmt_ctx,
                          std::function<int(int, void *&, int)> pop_pkt_cb)
            : se_fmt_ctx(se_fmt_ctx),
              pop_pkt_cb(pop_pkt_cb) {}

        virtual int run();

        int write_header();

        int write_tailer();
    };
}

#endif // SNAKE_EYE_WRITE_TASK_HPP
