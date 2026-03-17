#pragma once

#include "raylib.h"
#include "world/world.hpp"
#include "assets/AssetManager.hpp"
#include "pools/RenderPool.hpp"

namespace RenderSystem
{
    struct DrawCmd
    {
        Texture2D *tex;
        Rectangle src;
        Rectangle dst;
        int layer;
        Color color;
        Vector2 position;
    };



    void CollectRenders(World& world, AssetManager assets);
    void DrawPool(World& world, AssetManager assets);
    void DrawWorld(World& world, AssetManager& assets);
    void DrawLevel(World& world, int index);
    DrawCmd GetDrawCmd(World& world, int index, AssetManager assets);
    void DrawPath(World& w);
    void DrawGrid(World& w);
}
