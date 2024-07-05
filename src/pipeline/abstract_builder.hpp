#ifndef SNAKE_EYE_ABSTRACT_BUILDER_HPP
#define SNAKE_EYE_ABSTRACT_BUILDER_HPP

#include "pipeline.hpp"

namespace SnakeEye
{
    class SnakeEyeAbstractBuilder
    {
    protected:
        SnakeEyePipeline *pipeline = nullptr;

    public:
        virtual ~SnakeEyeAbstractBuilder()
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
}

#endif // SNAKE_EYE_ABSTRACT_BUILDER_HPP
