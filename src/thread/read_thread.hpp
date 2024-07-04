#ifndef SNAKE_EYE_READ_THREAD_HPP
#define SNAKE_EYE_READ_THREAD_HPP

#include "base_thread.hpp"
#include "../task/read_task.hpp"

namespace SnakeEye
{
    class SnakeEyeReadThread : public SnakeEyeBaseThread
    {
    private:
        SnakeEyeFormatContext *se_fmt_ctx = nullptr;

        std::function<int(int, void *, int)> push_pkt_cb;

    private:
        SnakeEyeReadTask *get_task()
        {
            return new SnakeEyeReadTask(this->se_fmt_ctx, this->push_pkt_cb);
        }

    public:
        SnakeEyeReadThread(SnakeEyeFormatContext *se_fmt_ctx,
                           std::function<int(int, void *, int)> push_pkt_cb)
            : se_fmt_ctx(se_fmt_ctx),
              push_pkt_cb(push_pkt_cb) {}
    };
}

#endif // SNAKE_EYE_READ_THREAD_HPP
