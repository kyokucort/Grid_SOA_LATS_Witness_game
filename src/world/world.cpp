#include "world.hpp"



void Init_World(World& world)
{
    world.entity.count = 0;
    world.active_level = 0;
}

void Update_World(World& world, float dt)
{
    MovementSystem::Update(world, dt);
    int _player = World_FindPlayer(world);
    Vector2 _playerpos = world.transform.pos[_player];
    int newLevel = World_FindLevelContaining(world, _playerpos);

    if (newLevel != -1 && newLevel != world.active_level)
    {
        world.active_level = newLevel;
    }
}


int World_FindLevelContaining(const World& world, Vector2 pos)
{
    for (int i = 0; i < world.loaded_levels.size(); i++){
        if (CheckCollisionPointRec(pos, world.loaded_levels[i].collider_editor))
            return i;
    }
    return -1;
}

int World_FindPlayer(const World& world)
{   
    for (int i = 0; i < MAX_ENTITIES; i++){
        if (world.entity.type[i] == EntityType::ENTITY_PLAYER)
            return i;
    }
    return -1;
}
