#pragma once
#include "world/world.hpp"

namespace GameplaySystem
{
    void Update(World& world);
    void HandleInteraction(World& world, int a, int b);
    void HandleCursorInteraction(World& world, int cursor, int other);
}



