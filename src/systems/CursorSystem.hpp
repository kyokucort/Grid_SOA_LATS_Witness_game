#pragma once

#include "raylib.h"
#include "spatial/Grid.hpp"

struct World;

struct Cursor {
    Vector2 position;
    Vector2 coords;
    Vector2 anchor;
    float move_speed;
    float slide_speed;
    bool can_turn;
    bool is_free;
    int hovered;
};


namespace CursorSystem
{
    void Init(World& world);
    void Update(World& w);
    void HandleClic(World& w, int index, Grid& grid);
    void HandleGridMovement(World& world, int index, Grid& grid);
    float GetCloserCellDistance(Vector2 position, Grid& grid);
    Vector2 GetMovementOnGrid();
    Vector2 GetCloserCell(Vector2 position, Grid& grid);
    void CheckNewCell(Vector2i cell, Grid& grid);
    Vector2 GetActiveCellCoords(Vector2 position, Grid& grid);
    int GetActiveCellIndex(Vector2 position, Grid& grid);
    void CheckWalls(World& world, int index, Grid& grid, Vector2i coords);
}
