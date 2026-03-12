#pragma once

#include "raylib.h"
#include "spatial/Grid.hpp"

struct World;



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



    void UpdateCursorPhysics(World& w, int index, Vector2 mouse_delta, float dt);
    void OnCursorEnterCell(World& w, int index, Vector2i new_cell);
    bool IsNearCellCenter(World& w, int index);
    void UpdatePath(Grid& grid, Vector2i next_cell);
    bool CanMovePath(Grid& grid, Vector2i next_cell);
    bool PathContains(const std::vector<Vector2i>& path, Vector2i cell);
}
