#ifndef PLAY_MANAGER_H
#define PLAY_MANAGER_H

#include <functional>

#include "../thread/play_thread.h"
#include "../element/play_frame.h"
#include "../element/clock.h"
#include "../util/osff_macro.h"

class PlayManager
{
private:
    PlayThreadContext *audio_play_thread_ctx = nullptr;

    PlayThreadContext *video_play_thread_ctx = nullptr;

    std::function<int(int, AVFrame *&, int)> pop_frm_cb_func;

    std::function<int(int, PlayFrame *, int)> push_frm_cb_func;

    PlayFrame *last_video_frm = nullptr;

    PlayFrame *cur_audio_frm = nullptr;

    PlayFrame *cur_video_frm = nullptr;

    PlayFrame *next_auido_frm = nullptr;

    PlayFrame *next_video_frm = nullptr;

    Clock *audio_clk = nullptr;

    Clock *video_clk = nullptr;

    double audio_frm_clk = 0.0;

    double video_frm_clk = 0.0;

public:
    PlayManager(std::function<int(int, AVFrame *&, int)> pop_frm_cb_func,
                std::function<int(int, PlayFrame *, int)> push_frm_cb_func)
        : pop_frm_cb_func(pop_frm_cb_func),
          push_frm_cb_func(push_frm_cb_func)
    {
        this->audio_clk = new Clock();
        this->video_clk = new Clock();
    }

    ~PlayManager()
    {
        if (this->audio_play_thread_ctx)
        {
            delete this->audio_play_thread_ctx;
            this->audio_play_thread_ctx = nullptr;
        }

        if (this->video_play_thread_ctx)
        {
            delete this->video_play_thread_ctx;
            this->video_play_thread_ctx = nullptr;
        }

        if (this->audio_clk)
        {
            delete this->audio_clk;
            this->audio_clk = nullptr;
        }

        if (this->video_clk)
        {
            delete this->video_clk;
            this->video_clk = nullptr;
        }
    }

    int start()
    {
        if (this->audio_play_thread_ctx)
        {
            this->audio_play_thread_ctx->start();
        }

        if (this->video_play_thread_ctx)
        {
            this->video_play_thread_ctx->start();
        }

        return 0;
    }

    int set_audio_play()
    {
        this->audio_play_thread_ctx = new PlayThreadContext(AUDIO_PLAY_STREAM_INDEX,
                                                            this->pop_frm_cb_func,
                                                            this->push_frm_cb_func);
        return 0;
    }

    int set_video_play()
    {
        this->video_play_thread_ctx = new PlayThreadContext(VIDEO_PLAY_STREAM_INDEX,
                                                            this->pop_frm_cb_func,
                                                            this->push_frm_cb_func);
        return 0;
    }

    int set_clock_at(int play_strm_idx,
                     double pts,
                     double time);

    int set_clock(int play_strm_idx,
                  double pts)
    {
        double time = av_gettime_relative() / 1000000.0;
        return this->set_clock_at(play_strm_idx, pts, time);
    }

    int get_clock(int play_strm_idx,
                  double &get_time);

    int set_frame_clock(int play_strm_idx,
                        double pts);

    int get_frame_clock(int play_strm_idx,
                        double &pts);
};

#endif // PLAY_MANAGER_H
