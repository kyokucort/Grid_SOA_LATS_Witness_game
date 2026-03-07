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
};


namespace CursorManager
{
    void Init(World& world, int index, Vector2 position);
    void Update(World& world, int index, Grid& grid);
    void HandleGridMovement(World& world, int index, Grid& grid);
    float GetCloserCellDistance(Vector2 position, Grid& grid);
    Vector2 GetMovementOnGrid();
    Vector2 GetCloserCell(Vector2 position, Grid& grid);
    Vector2 GetCurrentCellCoords(Vector2 position, Grid& grid);
    void CheckNewCell(Vector2 position, Grid& grid);
    Vector2 GetActiveCellCoords(Vector2 position, Grid& grid);
}
