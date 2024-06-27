#ifndef SNAKE_EYE_BASE_TASK_HPP
#define SNAKE_EYE_BASE_TASK_HPP

#include "../../static/marco.hpp"
#include "../../util/util.h"

namespace SnakeEye
{
    class SnakeEyeBaseTask
    {
    public:
        bool exit_flag = false;

        int status = 0;

        virtual ~SnakeEyeBaseTask() {}

        virtual int run() = 0;
    };
}

#endif // SNAKE_EYE_BASE_TASK_HPP
