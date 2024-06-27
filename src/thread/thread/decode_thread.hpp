#ifndef SNAKE_EYE_DECODE_THREAD_HPP
#define SNAKE_EYE_DECODE_THREAD_HPP

#include "base_thread.hpp"
#include "../task/decode_task.hpp"

namespace SnakeEye
{
    class SnakeEyeDecodeThread : public SnakeEyeBaseThread
    {
    private:
        SnakeEyeStreamContext *se_strm_ctx = nullptr;

        std::function<int(int, void *&, int)> pop_pkt_cb;

        std::function<int(int, void *, int)> push_frm_cb;

    private:
        virtual SnakeEyeBaseTask *get_task()
        {
            return new SnakeEyeDecodeTask(this->se_strm_ctx,
                                          this->pop_pkt_cb,
                                          this->push_frm_cb);
        }

    public:
        SnakeEyeDecodeThread(SnakeEyeStreamContext *se_strm_ctx,
                             std::function<int(int, void *&, int)> pop_pkt_cb,
                             std::function<int(int, void *, int)> push_frm_cb)
            : se_strm_ctx(se_strm_ctx),
              pop_pkt_cb(pop_pkt_cb),
              push_frm_cb(push_frm_cb) {}
    };
}

#endif // SNAKE_EYE_DECODE_THREAD_HPP
