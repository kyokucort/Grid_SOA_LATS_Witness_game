#pragma once

#include "raylib.h"
#include "spatial/Grid.hpp"
#include "assets/TextureID.hpp"

struct Level
{
    Rectangle area;
    Vector2 position;
    Vector2 center;
    Grid grid;
    Rectangle collider_editor;
    Color collider_color;
    TextureID texture_floor;
};

Level SpawnLevel(Vector2 pos, int width, int height);
void InitLevel(Level& lvl);
void LevelSetPosition(Level& lvl, Vector2 pos);
