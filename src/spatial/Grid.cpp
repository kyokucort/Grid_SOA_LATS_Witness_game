#include "Grid.hpp"
#include "assert.h"

void Init_WorldGrid(Grid& grid, int width, int height, float cell_size, Vector2 origin)
{
    grid.width = width;
    grid.height = height;
    grid.cell_size = cell_size;
    grid.origin = origin;

    grid.cells.resize(width * height);

    for (Cell& c : grid.cells)
    {
        c.count = 0;
        c.is_wall = false;
    }
}


// =========================
// Bounds
// =========================

bool GridIsInside(const Grid& grid, Vector2i pos)
{
    return pos.x >= 0 && pos.y >= 0 &&
           pos.x < grid.width &&
           pos.y < grid.height;
}

// =========================
// Access
// =========================

Cell& GridGetCellUnsafe(Grid& grid, Vector2i pos)
{
    return grid.cells[pos.y * grid.width + pos.x];
}

Cell* GridGetCell(Grid& grid, Vector2i pos)
{
    if (!GridIsInside(grid, pos))
        return nullptr;

    return &GridGetCellUnsafe(grid, pos);
}

// =========================
// Entity management
// =========================

void GridInsert(Grid& grid, Vector2i pos, int entity)
{
    Cell& cell = GridGetCellUnsafe(grid, pos);

    // (optionnel) éviter overflow
    if (cell.count >= 64)
        return;

    cell.entities[cell.count++] = entity;
}

void GridRemove(Grid& grid, Vector2i pos, int entity)
{
    Cell& cell = GridGetCellUnsafe(grid, pos);

    for (int i = 0; i < cell.count; ++i)
    {
        if (cell.entities[i] == entity)
        {
            // swap remove
            cell.entities[i] = cell.entities[cell.count - 1];
            cell.count--;
            return;
        }
    }
}



