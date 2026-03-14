#pragma once

#include "raylib.h"
#include "world/world.hpp"
#include "pools/EntityPool.hpp"
#include "pools/TransformPool.hpp"
//#include "core/job_types.hpp"

int CreateEntity(World& w);
void BaseEntitySetup(World& w, int index);
void DestroyEntity(World& w, int e);
int SpawnPlayer(World& world, Vector2i coords, JobType job);
TextureID GetTextureFromJob(JobType job);
int SpawnKey(World& w, Vector2i coords);
int SpawnDoor(World& w, Vector2i coords);
int SpawnWall(World& world, Vector2 pos);
int SpawnFloorGrass(World& world, Vector2 pos);
int SpawnCellConnector(World& world, Vector2 pos);
int SpawnCursor(World& world, Vector2 pos);
