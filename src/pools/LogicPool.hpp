#pragma once

#include "core/signal.hpp"

struct LogicRule
{
    uint32_t require;
    uint32_t forbid;
};

struct LogicPool
{
    bool has[MAX_ENTITIES];
    LogicRule rule[MAX_ENTITIES];
};
