#pragma once
#include "raylib.h"

struct World;

namespace MovementSystem
{
    void Update(World& world, float dt);
    void Update_Player(Vector2& pos, float dt);
    void Update_Cursor(World& world, int index);
}
