#include "world.hpp"
#include "systems/SpawnSystem.hpp"
#include "systems/MovementSystem.hpp"
#include "systems/CollisionSystem.hpp"
#include "systems/GameplaySystem.hpp"
#include "assert.h"


namespace WorldManager{

    void Init_World(World& world, CameraController::CameraController& camera_control)
    {
        world.entity.count = 0;
        world.active_level = 0;
        world.loaded_levels.push_back(SpawnLevel({0, 0}, 14, 8));
        world.loaded_levels.push_back(SpawnLevel({14*128, 0}, 14, 8));

        CameraController::Init(camera_control, SCREEN_WIDTH, SCREEN_HEIGHT);

// BORDEL POUR DEBUGGER UN INIT WORLD MAIS A REMPLACER PAR UN CHARGEMENT PROPRE DU FICHIER MONDE

        Vector2 _level_center = world.loaded_levels[world.active_level].position;
        _level_center.x += world.loaded_levels[world.active_level].grid.width * CELL_SIZE_WORLD/2;
        _level_center.y += world.loaded_levels[world.active_level].grid.height * CELL_SIZE_WORLD/2;
        CameraController::SetTarget(camera_control, _level_center);
        SpawnPlayer(world, world.loaded_levels[0].grid.cells[4].center, JobType::JOB_MAGE);
        SpawnCursor(world, {0, 0});
        CursorManager::Init(world, World_FindCursor(world), {64, 64});
        SpawnWall(world, world.loaded_levels[0].grid.cells[0].center);
        SpawnWall(world, world.loaded_levels[0].grid.cells[1].center);
        SpawnWall(world, world.loaded_levels[0].grid.cells[8].center);
        SpawnWall(world, world.loaded_levels[0].grid.cells[16].center);
        Init_Levels(world);
    }


    void Init_Levels(World& world)
    {
        for (int i = 0; i < world.loaded_levels.size(); i++){

            for (int c = 0; c < world.loaded_levels[i].grid.cells.size(); c++){
                Vector2 _pos = world.loaded_levels[i].grid.cells[c].center;
                world.loaded_levels[i].grid.cells[c].entities[0] = SpawnFloorGrass(world, _pos);
                SpawnCellConnector(world, _pos);
            }
        }
    }

    void Update_World(World& world, float dt)
    {
        assert(World_FindCursor(world) >= 0 && "We need the cursor to exist as an entity");
        
        MovementSystem::Update(world, dt);
        CollisionSystem::Update(world);
        GameplaySystem::Update(world);

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

    int World_FindCursor(const World& world)
    {   
        for (int i = 0; i < MAX_ENTITIES; i++){
            if (world.entity.type[i] == EntityType::ENTITY_CURSOR)
                return i;
        }
        return -1;
    }
}
