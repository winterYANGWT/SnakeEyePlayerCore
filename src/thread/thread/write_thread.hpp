#ifndef SNAKE_EYE_WRITE_THREAD_HPP
#define SNAKE_EYE_WRITE_THREAD_HPP

#include "base_thread.hpp"
#include "../task/write_task.hpp"

namespace SnakeEye
{
    class SnakeEyeWriteThreadContext : public SnakeEyeBaseThread
    {
    private:
        SnakeEyeFormatContext *se_fmt_ctx = nullptr;

        std::function<int(int, void *&, int)> pop_pkt_cb;

    private:
        virtual SnakeEyeBaseTask *get_task()
        {
            return new SnakeEyeWriteTask(this->se_fmt_ctx,
                                         this->pop_pkt_cb);
        }

    public:
        SnakeEyeWriteThreadContext(SnakeEyeFormatContext *se_fmt_ctx,
                                   std::function<int(int, void *&, int)> pop_pkt_cb)
            : se_fmt_ctx(se_fmt_ctx),
              pop_pkt_cb(pop_pkt_cb) {}
    };
}

#endif // SNAKE_EYE_WRITE_THREAD_HPP
