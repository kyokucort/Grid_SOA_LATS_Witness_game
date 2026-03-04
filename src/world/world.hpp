#pragma once

#include "pools/EntityPool.hpp"
#include "pools/TransformPool.hpp"
#include "pools/RenderPool.hpp"
#include "pools/ColliderPool.hpp"
#include "pools/MovePool.hpp"
#include "levels/level.hpp"
#include "spatial/Grid.hpp"
#include "cursor/Cursor.hpp"
#include <vector>

#include "systems/MovementSystem.hpp"


struct World
{
    std::vector<Level> loaded_levels;
    int active_level;
    EntityPool entity;
    TransformPool transform;
    MovePool move;
    RenderPool render;
    ColliderPool collider;
    Cursor cursor;
};


void Init_World(World& world);
void Update_World(World& world, float dt);
int World_FindLevelContaining(const World& world, Vector2 pos);
int World_FindPlayer(const World& world);
