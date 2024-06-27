#ifndef SNAKE_EYE_BASE_THREAD_HPP
#define SNAKE_EYE_BASE_THREAD_HPP

#include "../task/base_task.hpp"

#include <thread>

namespace SnakeEye
{
    int task_loop(SnakeEyeBaseTask *task);

    class SnakeEyeBaseThread
    {
    protected:
        bool thd_exit = false;

        int status = 0;

        std::thread *thd = nullptr;

        SnakeEyeBaseTask *task = nullptr;

    private:
        virtual SnakeEyeBaseTask *get_task() = 0;

    public:
        virtual ~SnakeEyeBaseThread()
        {
            this->end();
        }

        int start(void *opeaue = nullptr)
        {
            this->thd_exit = false;
            this->task = this->get_task();
            this->thd = new std::thread(&task_loop, this->task);
            return 0;
        }

        int end()
        {
            this->thd_exit = true;

            if (this->thd)
            {
                if (this->thd->joinable())
                {
                    this->thd->join();
                }

                delete this->thd;
                this->thd = nullptr;
            }

            return 0;
        }
    };
}

#endif // SNAKE_EYE_BASE_THREAD_WORKER_HPP
