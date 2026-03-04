#pragma once

#include "raylib.h"
#include <vector>

struct Cell
{
    int entities[16];
    int count;
};

struct Grid
{
    int width;
    int height;
    float cell_size;
    Vector2 position;

    std::vector<Cell> cells;
};

void InitGrid(Grid& grid, int width, int height, float cell_size, Vector2 position);
void GridInsert(Grid& g, int entity, Vector2 pos);
