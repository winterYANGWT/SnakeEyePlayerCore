#ifndef OSFF_ENCODE_THREAD_HPP
#define OSFF_ENCODE_THREAD_HPP

#include "base_thread.hpp"
#include "../task/encode_task.hpp"

class OSFFEncodeThread : public OSFFBaseThread
{
private:
    OSFFStreamContext *osff_strm_ctx = nullptr;

    std::function<int(int, void *&, int)> pop_frm_cb;

    std::function<int(int, void *, int)> push_pkt_cb;

private:
    virtual OSFFBaseTask *get_task()
    {
        return new OSFFEncodeTask(this->osff_strm_ctx,
                                  this->pop_frm_cb,
                                  this->push_pkt_cb);
    }

public:
    OSFFEncodeThread(OSFFStreamContext *osff_strm_ctx,
                     std::function<int(int, void *&, int)> pop_frm_cb,
                     std::function<int(int, void *, int)> push_pkt_cb)
        : osff_strm_ctx(osff_strm_ctx),
          pop_frm_cb(pop_frm_cb),
          push_pkt_cb(push_pkt_cb) {}
};

#endif // OSFF_ENCODE_THREAD_HPP