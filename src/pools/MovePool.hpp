#pragma once

#include "raylib.h"
#include "core/config.hpp"

struct MovePool
{
    Vector2 direction[MAX_ENTITIES];
    Vector2 anchor[MAX_ENTITIES];
    float cursor_base_speed[MAX_ENTITIES];
    float cursor_slide_speed[MAX_ENTITIES];
    bool cursor_can_turn[MAX_ENTITIES];
    bool cursor_is_free[MAX_ENTITIES];
};
