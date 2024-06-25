#include "play_frame_queue.hpp"

int OSFFPlayFrameQueue::reset_time(PlayFrame *frm)
{
    frm->pts = this->d_strm_tb * (double)frm->frm->pts;
    frm->duration = this->d_strm_tb * frm->frm->duration;
    return 0;
}
