#pragma once

#include "raylib.h"
#include "core/config.hpp"

struct HoverPool
{
    bool has[MAX_ENTITIES];
    bool hovered[MAX_ENTITIES];
};
