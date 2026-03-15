#pragma once

#include "core/signal.hpp"

struct SignalSource
{
    uint32_t signal;
};

struct SignalPool
{
    bool has[MAX_ENTITIES];
    SignalSource data[MAX_ENTITIES];
};

