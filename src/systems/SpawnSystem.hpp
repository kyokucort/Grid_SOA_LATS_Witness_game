#pragma once

#include "raylib.h"
#include "world/world.hpp"
#include "pools/EntityPool.hpp"
#include "pools/TransformPool.hpp"

int CreateEntity(EntityPool& pool);
void DestroyEntity(World& w, int e);
int SpawnPlayer(World& world, Vector2 pos);
int SpawnWall(World& world, Vector2 pos);
int SpawnFloorGrass(World& world, Vector2 pos);
