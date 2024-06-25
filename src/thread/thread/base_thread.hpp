#ifndef OSFF_BASE_THREAD_HPP
#define OSFF_BASE_THREAD_HPP

#include "../task/base_task.hpp"

int task_loop(OSFFBaseTask *task);

#include <thread>

class OSFFBaseThread
{
protected:
    bool thd_exit = false;

    int status = 0;

    std::thread *thd = nullptr;

    OSFFBaseTask *task = nullptr;

private:
    virtual OSFFBaseTask *get_task() = 0;

public:
    virtual ~OSFFBaseThread()
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

#endif // OSFF_BASE_THREAD_WORKER_HPP