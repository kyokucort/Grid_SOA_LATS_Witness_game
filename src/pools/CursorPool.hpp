#pragma once

#include "raylib.h"
#include "core/config.hpp"

struct CursorPool
{
    bool has[MAX_ENTITIES];
    Vector2 anchor[MAX_ENTITIES];
    float base_speed[MAX_ENTITIES];
    float slide_speed[MAX_ENTITIES];
    bool can_turn[MAX_ENTITIES];
    bool is_free[MAX_ENTITIES];
};
