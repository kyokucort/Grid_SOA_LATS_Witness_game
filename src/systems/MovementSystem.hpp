#pragma once
#include "raylib.h"
#include "modules/math/vector2i.hpp"
#include "spatial/Grid.hpp"

struct World;

namespace MovementSystem
{
    void Update(World& world, float dt);
    void Update_Player(World& w, int index, float dt);
    bool TryMoveEntity(World& w, int entity, Vector2i target);
}
