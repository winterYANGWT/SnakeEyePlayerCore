#include "data_manager.hpp"

SnakeEye::SnakeEyeDataManager::~SnakeEyeDataManager()
{
    for (auto it = this->queues.begin();
         it != this->queues.end();
         it++)
    {
        delete it->second;
    }

    this->queues.clear();
    // delete this->audio_play_frm_queue;
    // delete this->video_play_frm_queue;
}

int SnakeEye::SnakeEyeDataManager::add_data_queue(int queue_id,
                                                  SnakeEyeDataQueue *queue)
{
    auto queue_it = this->queues.find(queue_id);

    if (queue_it == this->queues.end())
    {
        this->queues[queue_id] = queue;
        this->queues_ref_count[queue_id] = 1;
        av_log(NULL, AV_LOG_INFO,
               "Created data queue #%d\n",
               queue_id);
    }
    else
    {
        this->queues_ref_count[queue_id] += 1;
    }

    return 0;
}

int SnakeEye::SnakeEyeDataManager::delete_data_queue(int queue_id)
{
    auto queue_it = this->queues.find(queue_id);

    if (queue_it == this->queues.end())
    {
        av_log(NULL, AV_LOG_ERROR,
               "Data queue #%d does not exist\n",
               queue_id);
        return AVERROR(EINVAL);
    }
    else
    {
        --this->queues_ref_count[queue_id];

        if (this->queues_ref_count[queue_id] == 0)
        {
            delete queue_it->second;
            this->queues.erase(queue_it);
            this->queues_ref_count.erase(this->queues_ref_count.find(queue_id));
            av_log(NULL, AV_LOG_INFO,
                   "Deleted data queue #%d\n",
                   queue_id);
        }

        return 0;
    }
}

int SnakeEye::SnakeEyeDataManager::get_data_queue(int queue_id,
                                                  SnakeEyeDataQueue *&queue)
{
    auto queue_it = this->queues.find(queue_id);

    if (queue_it == this->queues.end())
    {
        av_log(NULL, AV_LOG_ERROR,
               "Data queue #%d does not exist\n",
               queue_id);
        return AVERROR(EINVAL);
    }
    else
    {
        queue = queue_it->second;
        return 0;
    }
}

int SnakeEye::SnakeEyeDataManager::push_data(int queue_id,
                                             void *data,
                                             int timeout)
{
    SnakeEyeDataQueue *queue = nullptr;
    int ret = this->get_data_queue(queue_id, queue);

    if (ret < 0)
    {
        return ret;
    }
    else
    {
        return queue->push(data, timeout);
    }
}

int SnakeEye::SnakeEyeDataManager::pop_data(int queue_id,
                                            void *&data,
                                            int timeout)
{
    SnakeEyeDataQueue *queue = nullptr;
    int ret = this->get_data_queue(queue_id, queue);

    if (ret < 0)
    {
        return ret;
    }
    else
    {
        return queue->pop(data, timeout);
    }
}

// int SnakeEye::SnakeEyeDataManager::add_play_stream_queue(int strm_idx,
//                                                          AVRational strm_tb)
// {
//     if (strm_idx == AUDIO_PLAY_STREAM_INDEX &&
//         this->audio_play_frm_queue == nullptr)
//     {
//         this->audio_play_frm_queue = new SnakeEyePlayFrameQueue(strm_tb,
//                                                                 DEFAULT_FRAME_QUEUE_MAX_SIZE);
//     }
//     else if (strm_idx == VIDEO_PLAY_STREAM_INDEX &&
//              this->video_play_frm_queue == nullptr)
//     {
//         this->video_play_frm_queue = new SnakeEyePlayFrameQueue(strm_tb,
//                                                                 DEFAULT_FRAME_QUEUE_MAX_SIZE);
//     }
//     else
//     {
//         av_log(NULL, AV_LOG_ERROR,
//                "Invalid play stream id #%d to add play stream queue or corresponding play stream queue already exists\n",
//                strm_idx);
//         return AVERROR(EINVAL);
//     }

//     return 0;
// }

// int SnakeEye::SnakeEyeDataManager::delete_play_stream_queue(int strm_idx)
// {
//     if (strm_idx == AUDIO_PLAY_STREAM_INDEX &&
//         this->audio_play_frm_queue != nullptr)
//     {
//         delete this->audio_play_frm_queue;
//         this->audio_play_frm_queue = nullptr;
//     }
//     else if (strm_idx == VIDEO_PLAY_STREAM_INDEX &&
//              this->video_play_frm_queue != nullptr)
//     {
//         delete this->video_play_frm_queue;
//         this->video_play_frm_queue = nullptr;
//     }
//     else
//     {
//         av_log(NULL, AV_LOG_ERROR,
//                "Invaild play stream id #%d to delete play stream queue or corresponding play stream queue does not exist\n",
//                strm_idx);
//         return AVERROR(EINVAL);
//     }

//     return 0;
// }

// int SnakeEye::SnakeEyeDataManager::push_play_stream_frame(int strm_idx,
//                                                           SnakeEyePlayFrame *frm,
//                                                           int timeout)
// {
//     if (strm_idx == AUDIO_PLAY_STREAM_INDEX &&
//         this->audio_play_frm_queue != nullptr)
//     {
//         return this->audio_play_frm_queue->push_frame(frm, timeout);
//     }
//     else if (strm_idx == VIDEO_PLAY_STREAM_INDEX &&
//              this->video_play_frm_queue != nullptr)
//     {
//         return this->video_play_frm_queue->push_frame(frm, timeout);
//     }
//     else
//     {
//         av_log(NULL, AV_LOG_ERROR,
//                "Invalid stream id #%d to push play frame or corresponding play stream queue does not exist\n",
//                strm_idx);
//         return AVERROR(EINVAL);
//     }
// }

// int SnakeEye::SnakeEyeDataManager::pop_play_stream_frame(int strm_idx)
// {
//     if (strm_idx == AUDIO_PLAY_STREAM_INDEX &&
//         this->audio_play_frm_queue != nullptr)
//     {
//         return this->audio_play_frm_queue->pop_frame();
//     }
//     else if (strm_idx == VIDEO_PLAY_STREAM_INDEX &&
//              this->video_play_frm_queue != nullptr)
//     {
//         return this->video_play_frm_queue->pop_frame();
//     }
//     else
//     {
//         av_log(NULL, AV_LOG_ERROR,
//                "Invalid stream id #%d to pop play frame or corresponding play stream queue does not exist\n",
//                strm_idx);
//         return AVERROR(EINVAL);
//     }
// }

// int SnakeEye::SnakeEyeDataManager::peek_play_stream_frame(int strm_idx,
//                                                           SnakeEyePlayFrame *&frm)
// {
//     if (strm_idx == AUDIO_PLAY_STREAM_INDEX &&
//         this->audio_play_frm_queue != nullptr)
//     {
//         return this->audio_play_frm_queue->peek_frame(frm);
//     }
//     else if (strm_idx == VIDEO_PLAY_STREAM_INDEX &&
//              this->video_play_frm_queue != nullptr)
//     {
//         return this->video_play_frm_queue->peek_frame(frm);
//     }
//     else
//     {
//         av_log(NULL, AV_LOG_ERROR,
//                "Invalid stream id #%d to peek play frame or corresponding play stream queue does not exist\n",
//                strm_idx);
//         return AVERROR(EINVAL);
//     }
// }

// int SnakeEye::SnakeEyeDataManager::peek_last_play_stream_frame(int strm_idx,
//                                                                SnakeEyePlayFrame *&frm)
// {
//     if (strm_idx == AUDIO_PLAY_STREAM_INDEX &&
//         this->audio_play_frm_queue != nullptr)
//     {
//         return this->audio_play_frm_queue->peek_last_frame(frm);
//     }
//     else if (strm_idx == VIDEO_PLAY_STREAM_INDEX &&
//              this->video_play_frm_queue != nullptr)
//     {
//         return this->video_play_frm_queue->peek_last_frame(frm);
//     }
//     else
//     {
//         av_log(NULL, AV_LOG_ERROR,
//                "Invalid stream id #%d to peek last play frame or corresponding play stream queue does not exist\n",
//                strm_idx);
//         return AVERROR(EINVAL);
//     }
// }

// int SnakeEye::SnakeEyeDataManager::peek_next_play_stream_frame(int strm_idx,
//                                                                SnakeEyePlayFrame *&frm)
// {
//     if (strm_idx == AUDIO_PLAY_STREAM_INDEX &&
//         this->audio_play_frm_queue != nullptr)
//     {
//         return this->audio_play_frm_queue->peek_next_frame(frm);
//     }
//     else if (strm_idx == VIDEO_PLAY_STREAM_INDEX &&
//              this->video_play_frm_queue != nullptr)
//     {
//         return this->video_play_frm_queue->peek_next_frame(frm);
//     }
//     else
//     {
//         av_log(NULL, AV_LOG_ERROR,
//                "Invalid stream id #%d to peek next play frame or corresponding play stream queue does not exist\n",
//                strm_idx);
//         return AVERROR(EINVAL);
//     }
// }

// int SnakeEye::SnakeEyeDataManager::play_stream_queue_size(int strm_idx)
// {
//     if (strm_idx == AUDIO_PLAY_STREAM_INDEX &&
//         this->audio_play_frm_queue != nullptr)
//     {
//         return this->audio_play_frm_queue->size();
//     }
//     else if (strm_idx == VIDEO_PLAY_STREAM_INDEX &&
//              this->video_play_frm_queue != nullptr)
//     {
//         return this->video_play_frm_queue->size();
//     }
//     else
//     {
//         av_log(NULL, AV_LOG_ERROR,
//                "Invalid stream id #%d to get play stream queue size or corresponding play stream queue does not exist\n",
//                strm_idx);
//         return AVERROR(EINVAL);
//     }
// }

int SnakeEye::SnakeEyeDataManager::start()
{
    for (auto queue_it = this->queues.begin();
         queue_it != this->queues.end();
         queue_it++)
    {
        queue_it->second->enable();
    }

    // this->audio_play_frm_queue->start();
    // this->video_play_frm_queue->start();
    return 0;
}

int SnakeEye::SnakeEyeDataManager::end()
{
    for (auto queue_it = this->queues.begin();
         queue_it != this->queues.end();
         queue_it++)
    {
        queue_it->second->disable();
    }

    // this->audio_play_frm_queue->end();
    // this->video_play_frm_queue->end();
    return 0;
}
