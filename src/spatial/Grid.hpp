#pragma once

#include "raylib.h"
#include <vector>

struct Cell
{
    Vector2 coords;
    Vector2 center;
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
    std::vector<Vector2> path;
};

void InitGrid(Grid& grid, int width, int height, float cell_size, Vector2 position);
void Init_Cells(Grid& grid);
void GridInsert(Grid& g, int entity, Vector2 pos);
