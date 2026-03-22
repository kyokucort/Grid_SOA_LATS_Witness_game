#pragma once

#include "raylib.h"
#include "spatial/Grid.hpp"

struct World;



namespace CursorSystem
{
    void Init(World& world, int index);
    void Update(World& w);
    void HandleClic(World& w, int index, Grid& grid);
    int HasInteractible(World& w, Vector2i target_cell);
    void UpdateCursorPhysics(World& w, int index, Vector2 mouse_delta, float dt);
    void OnCursorEnterCell(World& w, int index, Vector2i new_cell);
    bool IsNearCellCenter(World& w, int index);
    void UpdatePath(std::vector<Vector2i>& path, Vector2i next_cell);
    bool CanMovePath(std::vector<Vector2i>& path, Vector2i next_cell);
    bool PathContains(const std::vector<Vector2i>& path, Vector2i cell);
    bool IsUndoing(std::vector<Vector2i>& path, Vector2i next_cell);
    void UpdateCursor(World& w, int entity, Vector2 mouse_delta, int interactible, float dt);
    void SyncCursorVisual(World& w, int entity, float dt);
}
