#include "data_queue.hpp"

int SnakeEye::SnakeEyeDataQueue::clear()
{
    std::unique_lock<std::mutex> lock(this->mtx);

    while (!this->queue.empty())
    {
        void *data = this->queue.front();
        this->queue.pop();
        this->destroy_data(data);
    }

    return 0;
}

int SnakeEye::SnakeEyeDataQueue::push(void *data, int timeout)
{
    this->preprocess(data);
    std::unique_lock<std::mutex> lock(this->mtx);

    if (timeout > 0)
    {
        if (!this->can_push.wait_for(lock,
                                     std::chrono::milliseconds(timeout),
                                     [this]
                                     { return (this->queue.size() < this->max_size ||
                                               this->exit); }))
        {
            return AVERROR(ETIMEDOUT);
        }
    }
    else
    {
        this->can_push.wait(lock,
                            [this]
                            { return (this->queue.size() < this->max_size ||
                                      this->exit); });
    }

    if (this->exit)
    {
        return AVERROR_EXIT;
    }

    this->queue.push(data);
    this->can_pop.notify_one();
    return 0;
}

int SnakeEye::SnakeEyeDataQueue::pop(void *&data,
                                     int timeout)
{
    std::unique_lock<std::mutex> lock(this->mtx);

    if (timeout > 0)
    {
        if (!this->can_pop.wait_for(lock,
                                    std::chrono::milliseconds(timeout),
                                    [this]
                                    { return (!this->queue.empty() ||
                                              this->exit); }))
        {
            return AVERROR(ETIMEDOUT);
        }
    }
    else
    {
        this->can_pop.wait(lock, [this]
                           { return (!this->queue.empty() ||
                                     this->exit); });
    }

    if (this->exit)
    {
        return AVERROR_EXIT;
    }

    data = this->queue.front();
    this->queue.pop();
    this->postprocess(data);
    this->can_push.notify_one();
    return 0;
}
