#include "base_thread.hpp"

int SnakeEye::task_loop(SnakeEyeBaseTask *task)
{
    while (!task->exit_flag)
    {
        task->run();
    }

    return task->status;
}
