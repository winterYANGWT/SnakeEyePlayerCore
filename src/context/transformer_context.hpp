#ifndef OSFF_TRANSFORMER_CONTEXT_HPP
#define OSFF_TRANSFORMER_CONTEXT_HPP

#include <unordered_map>

#include "../transformer/transformer.hpp"

class OSFFTransformerContext
{
public:
    int dstrm_idx = -1;

    std::unordered_map<int, OSFFTransformer *> trfms;
};

#endif // OSFF_TRANSFORMER_CONTEXT_HPP