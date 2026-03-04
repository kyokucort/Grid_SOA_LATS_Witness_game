#pragma once

#include "raylib.h"
#include "assets/TextureID.hpp"

struct RenderPool
{
    bool active[MAX_ENTITIES];
    TextureID texture[MAX_ENTITIES];
    Rectangle src[MAX_ENTITIES];
    Color color[MAX_ENTITIES];
    int layer[MAX_ENTITIES];
};
