#pragma once

#include "raylib.h"
#include "core/config.hpp"
#include <vector>
#include "modules/math/vector2i.hpp"

struct Path
{
    std::vector<Vector2i> points;
};

struct PathPool
{
    bool has[MAX_ENTITIES];
    Path path[MAX_ENTITIES];
};
