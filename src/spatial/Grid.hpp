#pragma once

#include "raylib.h"
#include "core/config.hpp"
#include <vector>


struct Cell
{
    Vector2 coords;
    Vector2 center;
    int entities[MAX_ENTITIES_PER_CELL];
    int count;
    bool is_wall;
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
Cell& Grid_GetCell(Grid& grid, int x, int y);
void CellInsertEntity(Grid& grid, int entity, Vector2 coords);
void Cell_AddEntity(Cell& cell, int entity);
void Cell_RemoveEntity(Cell& cell, int entity);
int GetCellFromCoords(Grid& grid, int x, int y);
