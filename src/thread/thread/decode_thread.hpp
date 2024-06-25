#ifndef OSFF_DECODE_THREAD_HPP
#define OSFF_DECODE_THREAD_HPP

#include "base_thread.hpp"
#include "../task/decode_task.hpp"

class OSFFDecodeThread : public OSFFBaseThread
{
private:
    OSFFStreamContext *osff_strm_ctx = nullptr;

    std::function<int(int, void *&, int)> pop_pkt_cb;

    std::function<int(int, void *, int)> push_frm_cb;

private:
    virtual OSFFBaseTask *get_task()
    {
        return new OSFFDecodeTask(this->osff_strm_ctx,
                                  this->pop_pkt_cb,
                                  this->push_frm_cb);
    }

public:
    OSFFDecodeThread(OSFFStreamContext *osff_strm_ctx,
                     std::function<int(int, void *&, int)> pop_pkt_cb,
                     std::function<int(int, void *, int)> push_frm_cb)
        : osff_strm_ctx(osff_strm_ctx),
          pop_pkt_cb(pop_pkt_cb),
          push_frm_cb(push_frm_cb) {}
};

#endif // OSFF_DECODE_THREAD_HPP