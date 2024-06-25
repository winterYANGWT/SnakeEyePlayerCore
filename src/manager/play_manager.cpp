#include "play_manager.h"

int PlayManager::set_clock_at(int play_strm_idx, double pts, double time)
{
    if (play_strm_idx == AUDIO_PLAY_STREAM_INDEX)
    {
        this->audio_clk->set_clock_at(pts, time);
    }
    else if (play_strm_idx == VIDEO_PLAY_STREAM_INDEX)
    {
        this->video_clk->set_clock_at(pts, time);
    }
    else
    {
        av_log(NULL, AV_LOG_ERROR,
               "Invalid play stream index: %d\n",
               play_strm_idx);
        return AVERROR(EINVAL);
    }

    return 0;
}

int PlayManager::get_clock(int play_strm_idx,
                           double &get_time)
{
    if (play_strm_idx == AUDIO_PLAY_STREAM_INDEX)
    {
        get_time = this->audio_clk->get_clock();
    }
    else if (play_strm_idx == VIDEO_PLAY_STREAM_INDEX)
    {
        get_time = this->video_clk->get_clock();
    }
    else
    {
        av_log(NULL, AV_LOG_ERROR,
               "invalid play stream index: %d\n",
               play_strm_idx);
        return AVERROR(EINVAL);
    }

    return 0;
}

int PlayManager::set_frame_clock(int play_strm_idx,
                                 double pts)
{
    if (play_strm_idx == AUDIO_PLAY_STREAM_INDEX)
    {
        this->audio_frm_clk = pts;
    }
    else if (play_strm_idx == VIDEO_PLAY_STREAM_INDEX)
    {
        this->video_frm_clk = pts;
    }
    else
    {
        av_log(NULL, AV_LOG_ERROR,
               "Invalid play stream index: %d\n",
               play_strm_idx);
        return AVERROR(EINVAL);
    }

    return 0;
}

int PlayManager::get_frame_clock(int play_strm_idx, double &pts)
{
    if (play_strm_idx == AUDIO_PLAY_STREAM_INDEX)
    {
        pts = this->audio_frm_clk;
    }
    else if (play_strm_idx == VIDEO_PLAY_STREAM_INDEX)
    {
        pts = this->video_frm_clk;
    }
    else
    {
        av_log(NULL, AV_LOG_ERROR,
               "Invalid play stream index: %d\n",
               play_strm_idx);
        return AVERROR(EINVAL);
    }
    return 0;
}
