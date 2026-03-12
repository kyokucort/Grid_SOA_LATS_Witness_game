#pragma once

#include "raylib.h"
#include "core/config.hpp"

enum class CursorAxis
{
    AXIS_NONE,
    AXIS_HORIZONTAL,
    AXIS_VERTICAL
};

struct CursorPool
{
    bool has[MAX_ENTITIES];
    Vector2 pos[MAX_ENTITIES];
    Vector2 velocity[MAX_ENTITIES];
    Vector2i cell[MAX_ENTITIES];
    CursorAxis axis[MAX_ENTITIES];
    

    Vector2 anchor[MAX_ENTITIES];
    float base_speed[MAX_ENTITIES];
    float slide_speed[MAX_ENTITIES];
    bool can_turn[MAX_ENTITIES];
    bool is_free[MAX_ENTITIES];
};
