#pragma once
#include "raylib.h"
#include "modules/math/vector2i.hpp"
#include "spatial/Grid.hpp"

struct World;

namespace MovementSystem
{
    void Update(World& world, float dt);
    void Update_Player(World& w, int index, float dt);
    void Update_Cursor(World& world, int index);
    void MovePlayer(World& w, int player, Vector2i old_cell, Vector2i new_cell);
    void Grid_MoveEntity(int entity, Cell& from, Cell& to);
}
