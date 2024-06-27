#ifndef SNAKE_EYE_TRANSFORMER_CONTEXT_HPP
#define SNAKE_EYE_TRANSFORMER_CONTEXT_HPP

#include <unordered_map>

#include "../transformer/transformer.hpp"

namespace SnakeEye
{
    class SnakeEyeTransformerContext
    {
    public:
        int dstrm_idx = -1;

        std::unordered_map<int, SnakeEyeTransformer *> trfms;
    };
}

#endif // SNAKE_EYE_TRANSFORMER_CONTEXT_HPP
