#pragma once

#include "pools/EntityPool.hpp"
#include "pools/TransformPool.hpp"
#include "pools/RenderPool.hpp"
#include "pools/ColliderPool.hpp"
#include "pools/MovePool.hpp"
#include "pools/CursorPool.hpp"
#include "pools/HoverPool.hpp"
#include "pools/JobPool.hpp"
#include "pools/PathPool.hpp"
#include "pools/SignalPool.hpp"
#include "pools/ModifierPool.hpp"
#include "pools/LogicPool.hpp"
#include "spatial/Grid.hpp"
#include "camera/CameraController.hpp"
#include <vector>
#include "editor/EditorState.hpp"


struct CollisionEvent
{
    int a;
    int b;
};

struct World
{
    EditorState editor;
    Grid global_grid;
    int active_level;
    Vector2i cursor_cell;
    int active_interactible;
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
    PathPool path;

    SignalPool signal;
    ModifierPool modifier;
    LogicPool logic;
    uint32_t state;
};

namespace WorldManager
{
    void Init_World(World& world, CameraController::CameraController& camera_control);
    void Update_World(World& world, CameraController::CameraController cam, float dt);
    int World_FindPlayer(const World& world);
    int World_FindCursor(const World& world);
    void MoveEntity(World& w, int e, Vector2i new_cell);
    void RemoveEntity(World& w, int e);
}
