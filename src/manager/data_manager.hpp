#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

/**
 * @file data_manager.h
 * @brief This file contains the declaration of the DataManager class.
 */

#include <unordered_map>

#include "../queue/data_queue.hpp"
#include "../queue/play_frame_queue.hpp"

/**
 * @class DataManager
 * @brief Class responsible for managing data.
 */
class DataManager
{
private:
    std::unordered_map<int, OSFFDataQueue *> queues;

    std::unordered_map<int, int> queues_ref_count;

    OSFFPlayFrameQueue *audio_play_frm_queue = nullptr;

    OSFFPlayFrameQueue *video_play_frm_queue = nullptr;

public:
    ~DataManager();

    int add_data_queue(int queue_id,
                       OSFFDataQueue *queue);

    int delete_data_queue(int queue_id);

    int get_data_queue(int queue_id,
                       OSFFDataQueue *&queue);

    int push_data(int queue_id,
                  void *data,
                  int timeout);

    int pop_data(int queue_id,
                 void *&data,
                 int timeout);

    int add_play_stream_queue(int strm_idx,
                              AVRational strm_tb);

    int delete_play_stream_queue(int strm_idx);

    int push_play_stream_frame(int strm_idx,
                               PlayFrame *frm,
                               int timeout);

    int pop_play_stream_frame(int strm_idx);

    int peek_play_stream_frame(int strm_idx,
                               PlayFrame *&frm);

    int peek_last_play_stream_frame(int strm_idx,
                                    PlayFrame *&frm);

    int peek_next_play_stream_frame(int strm_idx,
                                    PlayFrame *&frm);

    int play_stream_queue_size(int strm_idx);

    int start();

    int end();
};

#endif // DATA_MANAGER_H
