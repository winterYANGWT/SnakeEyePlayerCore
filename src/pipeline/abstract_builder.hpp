#ifndef OSFF_ABSTRACT_BUILDER_HPP
#define OSFF_ABSTRACT_BUILDER_HPP

#include "pipeline.hpp"

class OSFFAbstractBuilder
{
protected:
    OSFFPipeline *pipeline = nullptr;

public:
    virtual ~OSFFAbstractBuilder()
    {
        if (this->pipeline != nullptr)
        {
            delete this->pipeline;
        }
    }

    virtual int create_read() = 0;

    virtual int create_read_transfer() = 0;

    virtual int create_decode() = 0;

    virtual int create_transform() = 0;

    virtual int create_encode() = 0;

    virtual int create_write_transfer() = 0;

    virtual int create_write() = 0;
};

#endif // OSFF_ABSTRACT_BUILDER_HPP