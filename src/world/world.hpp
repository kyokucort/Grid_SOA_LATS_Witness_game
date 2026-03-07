#pragma once

#include "pools/EntityPool.hpp"
#include "pools/TransformPool.hpp"
#include "pools/RenderPool.hpp"
#include "pools/ColliderPool.hpp"
#include "pools/MovePool.hpp"
#include "levels/level.hpp"
#include "spatial/Grid.hpp"
#include "cursor/Cursor.hpp"
#include "camera/CameraController.hpp"
#include <vector>


struct CollisionEvent
{
    int a;
    int b;
};

struct World
{
    std::vector<Level> loaded_levels;
    int active_level;
    EntityPool entity;
    TransformPool transform;
    MovePool move;
    RenderPool render;
    ColliderPool collider;
    std::vector<CollisionEvent> collision_events;
    Cursor cursor;
};

namespace WorldManager
{
    void Init_World(World& world, CameraController::CameraController& camera_control);
    void Init_Levels(World& world);
    void Update_World(World& world, float dt);
    int World_FindLevelContaining(const World& world, Vector2 pos);
    int World_FindPlayer(const World& world);
    int World_FindCursor(const World& world);
}
