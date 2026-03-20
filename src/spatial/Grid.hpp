#pragma once

#include "raylib.h"
#include "core/config.hpp"
#include <vector>
#include "modules/math/vector2i.hpp"


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
    Vector2 origin;

    std::vector<Cell> cells;
    std::vector<Vector2i> path;
};


void Init_WorldGrid(Grid& grid, int width, int height, float cell_size, Vector2 origin);

// =========================
// Bounds
// =========================

bool GridIsInside(const Grid& grid, Vector2i pos);

// =========================
// Safe access (recommended)
// =========================

Cell* GridGetCell(Grid& grid, Vector2i pos);

// =========================
// Unsafe access (fast)
// ⚠️ assume pos is valid
// =========================

Cell& GridGetCellUnsafe(Grid& grid, Vector2i pos);

// =========================
// Entity management
// =========================

void GridInsert(Grid& grid, Vector2i pos, int entity);
void GridRemove(Grid& grid, Vector2i pos, int entity);
