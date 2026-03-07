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
    void DrawPlayer(Vector2 position, Vector2 size, Color color);
    void DrawWorld(World& world, AssetManager& assets);
    void DrawLevel(World& world, int index);
    void DrawCursor(World& world);
    DrawCmd GetDrawCmd(World& world, int index, AssetManager assets);
}
