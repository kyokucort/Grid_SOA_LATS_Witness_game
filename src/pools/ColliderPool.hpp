#pragma once

#include "raylib.h"
#include "core/config.hpp"

struct ColliderPool
{
    Rectangle editor[MAX_ENTITIES];
    Rectangle bounds[MAX_ENTITIES];
    bool has[MAX_ENTITIES];
};
