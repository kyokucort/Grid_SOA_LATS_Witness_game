#pragma once

#include "pools/EntityPool.hpp"
#include "pools/TransformPool.hpp"
#include "pools/RenderPool.hpp"
#include "pools/ColliderPool.hpp"
#include "pools/MovePool.hpp"
#include "pools/CursorPool.hpp"
#include "pools/HoverPool.hpp"
#include "pools/JobPool.hpp"
#include "levels/level.hpp"
#include "spatial/Grid.hpp"
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
    Vector2i cursor_cell;
    Vector2 mouse_world;
    EntityPool entity;
    TransformPool transform;
    MovePool move;
    RenderPool render;
    ColliderPool collider;
    std::vector<CollisionEvent> collision_events;
    JobPool job;
    CursorPool cursor;
    HoverPool hover;
};

namespace WorldManager
{
    void Init_World(World& world, CameraController::CameraController& camera_control);
    void Init_Levels(World& world);
    void Update_World(World& world, CameraController::CameraController cam, float dt);
    int World_FindLevelContaining(const World& world, Vector2 pos);
    int World_FindPlayer(const World& world);
    int World_FindCursor(const World& world);
}
