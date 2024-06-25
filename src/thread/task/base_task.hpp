#ifndef OSFF_BASE_TASK_HPP
#define OSFF_BASE_TASK_HPP

#include "../../util/osff_macro.h"
#include "../../util/util.h"

class OSFFBaseTask
{
public:
    bool exit_flag = false;

    int status = 0;

    virtual ~OSFFBaseTask() {}

    virtual int run() = 0;
};

#endif // OSFF_BASE_TASK_HPP