#pragma once

#include "raylib.h"
#include "spatial/Grid.hpp"

struct Level
{
    Vector2 position;
    Vector2 center;
    Grid grid;
    Rectangle collider_editor;
    Color collider_color;
};

Level SpawnLevel(Vector2 pos, int width, int height);
void InitLevel(Level& lvl);
void LevelSetPosition(Level& lvl, Vector2 pos);
