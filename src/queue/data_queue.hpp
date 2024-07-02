#ifndef SNAKE_EYE_DATA_QUEUE_HPP
#define SNAKE_EYE_DATA_QUEUE_HPP

extern "C"
{
#include <libavutil/error.h>
}
#include <queue>
#include <mutex>
#include <condition_variable>
#include <string>

#include "../static/macro.hpp"

namespace SnakeEye
{
    class SnakeEyeDataQueue
    {
    protected:
        std::queue<void *> queue;

        int max_size = -1;

        bool exit = false;

        std::mutex mtx;

        std::condition_variable can_push;

        std::condition_variable can_pop;

    public:
        const static std::string type;

    public:
        SnakeEyeDataQueue(int max_size = -1)
        {
            if (max_size <= 0)
            {
                this->max_size = DEFAULT_QUEUE_MAX_SIZE;
            }
            else
            {
                this->max_size = max_size;
            }
        }

        virtual ~SnakeEyeDataQueue()
        {
            this->disable();
        }

        virtual int preprocess(void *data) { return 0; }

        virtual int postprocess(void *data) { return 0; }

        virtual int destroy_data(void *data) { return 0; }

        int clear();

        int push(void *data,
                 int timeout);

        int pop(void *&data,
                int timeout);

        int size()
        {
            return this->queue.size();
        }

        int enable()
        {
            this->exit = false;
            return 0;
        }

        int disable()
        {
            this->exit = true;
            this->can_push.notify_all();
            this->can_pop.notify_all();
            this->clear();
            return 0;
        }
    };

    const std::string SnakeEyeDataQueue::type = "SnakeEyeDataQueue";
}

#endif // SNAKE_EYE_DATA_QUEUE_HPP
