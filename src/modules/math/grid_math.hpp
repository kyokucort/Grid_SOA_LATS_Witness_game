#pragma once

#include "raylib.h"
#include "vector2i.hpp"
#include <cmath>

inline Vector2i WorldToCell(Vector2 world_pos,
                            Vector2 grid_origin,
                            float cell_size)
{
    Vector2 local =
    {
        world_pos.x - grid_origin.x,
        world_pos.y - grid_origin.y
    };

    return {
        (int)floor(local.x / cell_size),
        (int)floor(local.y / cell_size)
    };
}

inline Vector2 CellToWorld(Vector2i cell,
                           Vector2 grid_origin,
                           float cell_size)
{
    return {
        grid_origin.x + cell.x * cell_size,
        grid_origin.y + cell.y * cell_size
    };
}

inline Vector2 CellCenter(Vector2i cell,
                          Vector2 grid_origin,
                          float cell_size)
{
    return {
        grid_origin.x + (cell.x + 0.5f) * cell_size,
        grid_origin.y + (cell.y + 0.5f) * cell_size
    };
}

inline bool IsCellInside(Vector2i cell,
                         int width,
                         int height)
{
    return
        cell.x >= 0 &&
        cell.y >= 0 &&
        cell.x < width &&
        cell.y < height;
}

inline int CellIndex(Vector2i cell, int width)
{
    return cell.y * width + cell.x;
}

static const Vector2i CARDINAL_DIRS[4] =
{
    {1,0},
    {-1,0},
    {0,1},
    {0,-1}
};
