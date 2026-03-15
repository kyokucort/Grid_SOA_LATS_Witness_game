#pragma once

#include "core/signal.hpp"

struct SignalModifier
{
    uint32_t add;
    uint32_t remove;
};

struct ModifierPool
{
    bool has[MAX_ENTITIES];
    SignalModifier data[MAX_ENTITIES];
};
