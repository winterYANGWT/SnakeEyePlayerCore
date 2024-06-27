#ifndef SNAKE_EYE_ENCODE_THREAD_HPP
#define SNAKE_EYE_ENCODE_THREAD_HPP

#include "base_thread.hpp"
#include "../task/encode_task.hpp"

namespace SnakeEye
{
    class SnakeEyeEncodeThread : public SnakeEyeBaseThread
    {
    private:
        SnakeEyeStreamContext *se_strm_ctx = nullptr;

        std::function<int(int, void *&, int)> pop_frm_cb;

        std::function<int(int, void *, int)> push_pkt_cb;

    private:
        virtual SnakeEyeBaseTask *get_task()
        {
            return new SnakeEyeEncodeTask(this->se_strm_ctx,
                                          this->pop_frm_cb,
                                          this->push_pkt_cb);
        }

    public:
        SnakeEyeEncodeThread(SnakeEyeStreamContext *se_strm_ctx,
                             std::function<int(int, void *&, int)> pop_frm_cb,
                             std::function<int(int, void *, int)> push_pkt_cb)
            : se_strm_ctx(se_strm_ctx),
              pop_frm_cb(pop_frm_cb),
              push_pkt_cb(push_pkt_cb) {}
    };
}

#endif // SNAKE_EYE_ENCODE_THREAD_HPP
