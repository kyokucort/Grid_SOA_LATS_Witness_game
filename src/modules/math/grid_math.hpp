#pragma once

#include "raylib.h"
#include "vector2i.hpp"
#include <cmath>


inline Vector2i WorldToCell(const Level& level, Vector2 world_pos)
{
    Vector2 local =
    {
        world_pos.x - level.position.x,
        world_pos.y - level.position.y
    };

    return {
        (int)floor(local.x / level.grid.cell_size),
        (int)floor(local.y / level.grid.cell_size)
    };
}

inline Vector2 CellToWorld(const Level& level, Vector2i cell)
{
    return {
        level.position.x + cell.x * level.grid.cell_size,
        level.position.y + cell.y * level.grid.cell_size
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

inline Vector2i CellCoords(int index, int width)
{
    return {
        index % width,
        index / width
    };
}

static const Vector2i CARDINAL_DIRS[4] =
{
    {1,0},
    {-1,0},
    {0,1},
    {0,-1}
};
