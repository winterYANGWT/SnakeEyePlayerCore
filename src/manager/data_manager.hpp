#ifndef SNAKE_EYE_DATA_MANAGER_HPP
#define SNAKE_EYE_DATA_MANAGER_HPP

/**
 * @file data_manager.hpp
 * @brief This file contains the declaration of the SnakeEyeDataManager class.
 */

#include <unordered_map>

#include "../queue/data_queue.hpp"
#include "../queue/play_frame_queue.hpp"

namespace SnakeEye
{
    /**
     * @class SnakeEyeDataManager
     * @brief Class responsible for managing data.
     */
    class SnakeEyeDataManager
    {
    private:
        std::unordered_map<int, SnakeEyeDataQueue *> queues;

        std::unordered_map<int, int> queues_ref_count;

        // SnakeEyePlayFrameQueue *audio_play_frm_queue = nullptr;

        // SnakeEyePlayFrameQueue *video_play_frm_queue = nullptr;

    public:
        ~SnakeEyeDataManager();

        int add_data_queue(int queue_id,
                           SnakeEyeDataQueue *queue);

        int delete_data_queue(int queue_id);

        int get_data_queue(int queue_id,
                           SnakeEyeDataQueue *&queue);

        int push_data(int queue_id,
                      void *data,
                      int timeout);

        int pop_data(int queue_id,
                     void *&data,
                     int timeout);

        // int add_play_stream_queue(int strm_idx,
        //                           AVRational strm_tb);

        // int delete_play_stream_queue(int strm_idx);

        // int push_play_stream_frame(int strm_idx,
        //                            SnakeEyePlayFrame *frm,
        //                            int timeout);

        // int pop_play_stream_frame(int strm_idx);

        // int peek_play_stream_frame(int strm_idx,
        //                            SnakeEyePlayFrame *&frm);

        // int peek_last_play_stream_frame(int strm_idx,
        //                                 SnakeEyePlayFrame *&frm);

        // int peek_next_play_stream_frame(int strm_idx,
        //                                 SnakeEyePlayFrame *&frm);

        // int play_stream_queue_size(int strm_idx);

        int start();

        int end();
    };
}

#endif // SNAKE_EYE_DATA_MANAGER_HPP
