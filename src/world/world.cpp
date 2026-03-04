#include "world.hpp"



void Init_World(World& world)
{
    world.entity.count = 0;
    world.active_level = 0;
}

void Update_World(World& world, float dt)
{
    MovementSystem::Update(world, dt);
}
