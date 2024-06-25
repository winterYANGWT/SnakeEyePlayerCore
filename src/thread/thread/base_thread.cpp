#include "base_thread.hpp"

int task_loop(OSFFBaseTask *task)
{
    while (!task->exit_flag)
    {
        task->run();
    }

    return task->status;
}