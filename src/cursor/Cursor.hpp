#pragma once

#include "raylib.h"
#include "spatial/Grid.hpp"

struct Cursor {
    Vector2 position;
    Vector2 anchor;
    float move_speed;
    float slide_speed;
    bool can_turn;
};


namespace CursorManager
{
    void Init(Cursor& cursor, Grid& grid);
    void Update(Cursor &cursor, Grid grid);
    float GetCloserCellDistance(Vector2 position, Grid grid);
    Vector2 GetMovementOnGrid();
    Vector2 GetCloserCell(Vector2 position, Grid grid);
}
