#pragma once
#include <stdint.h>

enum Signal : uint32_t
{
    SIGNAL_NONE  = 0,
    SIGNAL_KEY   = 1 << 0,
    SIGNAL_FIRE  = 1 << 1,
    SIGNAL_ICE   = 1 << 2,
    SIGNAL_BLUE  = 1 << 3
};
