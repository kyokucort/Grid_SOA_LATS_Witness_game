#pragma once

#include "raylib.h"
#include <array>
#include "pools/EntityPool.hpp"
#include "pools/TransformPool.hpp"
#include "assets/TextureID.hpp"
#include "core/signal.hpp"
#include "core/archetypes.hpp"

struct World;


using ArchetypeBuilder = void(*)(World&, int, Vector2i);


void InitSpawnSystem();

int CreateFromArchetype(World& w, Archetype type, Vector2i cell);

void BuildPlayer(World& w, int e, Vector2i cell);
void BuildCursor(World& w, int e, Vector2i cell);
void BuildKey(World& w, int e, Vector2i cell);

void AddEntity(World& w, int e, EntityType type);
void AddTransform(World& w, int e, Vector2 pos, Vector2 size, Vector2 scale, Vector2i cell);
void AddCollider(World& w, int e, Vector2 pos, Vector2 size);
void AddRender(World& w, int e, TextureID id_texture, Rectangle src);
void AddPath(World& w, int e);
void AddCursor(World& w, int e);
void AddHover(World& w, int e);
void AddSignal(World& w, int e, Signal signal);



//############################
//A EFFACER
//
//
//

int CreateEntity(World& w);
void BaseEntitySetup(World& w, int index);
void DestroyEntity(World& w, int e);
int SpawnDoor(World& w, Vector2i coords);
int SpawnWall(World& world, Vector2 pos);
int SpawnFloorGrass(World& world, Vector2 pos);
int SpawnCellConnector(World& world, Vector2 pos);
