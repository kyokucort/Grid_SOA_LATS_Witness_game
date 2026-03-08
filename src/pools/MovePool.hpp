#pragma once

#include "raylib.h"
#include "core/config.hpp"

struct MovePool
{
    Vector2 direction[MAX_ENTITIES];
    Vector2 anchor[MAX_ENTITIES];
};
