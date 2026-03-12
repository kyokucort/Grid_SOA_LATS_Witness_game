#pragma once

#include "raylib.h"
#include "core/config.hpp"
#include "modules/math/vector2i.hpp"

struct TransformPool
{
    Vector2 pos[MAX_ENTITIES];
    Vector2 size[MAX_ENTITIES];
    Vector2 scale[MAX_ENTITIES];
    Vector2i cell[MAX_ENTITIES];
};
