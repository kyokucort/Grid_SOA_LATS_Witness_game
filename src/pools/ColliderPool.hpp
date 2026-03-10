#pragma once

#include "raylib.h"
#include "core/config.hpp"

struct ColliderPool
{
    bool has[MAX_ENTITIES];
    Rectangle editor[MAX_ENTITIES];
    Rectangle bounds[MAX_ENTITIES];
    bool is_under_cursor[MAX_ENTITIES];
};
