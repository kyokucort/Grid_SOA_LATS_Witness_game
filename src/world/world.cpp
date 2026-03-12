#include "world.hpp"
#include "systems/SpawnSystem.hpp"
#include "systems/CursorSystem.hpp"
#include "systems/MovementSystem.hpp"
#include "systems/CollisionSystem.hpp"
#include "systems/GameplaySystem.hpp"
#include "systems/HoverSystem.hpp"
#include "modules/math/grid_math.hpp"
#include "assert.h"


namespace WorldManager{

    void Init_World(World& world, CameraController::CameraController& camera_control)
    {
        world.entity.count = 0;
        world.active_level = 0;
        world.loaded_levels.push_back(SpawnLevel({0, 0}, 14, 8));
        world.loaded_levels.push_back(SpawnLevel({14*128, 0}, 14, 8));

        CameraController::Init(camera_control, SCREEN_WIDTH, SCREEN_HEIGHT);
        CursorSystem::Init(world);
        Init_Levels(world);

// BORDEL POUR DEBUGGER UN INIT WORLD MAIS A REMPLACER PAR UN CHARGEMENT PROPRE DU FICHIER MONDE

        SpawnWall(world, world.loaded_levels[0].grid.cells[0].center);
    }



// Separer la logic de Level et tout dans un Level.cpp ou pas de Level.cpp du tout ?

    void Init_Levels(World& world)
    {
        for (int i = 0; i < world.loaded_levels.size(); i++)
        {
            Grid& _grid = world.loaded_levels[i].grid;
            for (int c = 0; c < _grid.cells.size(); c++){
                Cell& _cell = _grid.cells[c];
                
                Cell_AddEntity(_cell, SpawnFloorGrass(world, _cell.center));
                Cell_AddEntity(_cell, SpawnCellConnector(world, _cell.center));
            }

        }
        Grid& _grid = world.loaded_levels[world.active_level].grid;
        Cell& _cell_start = Grid_GetCell(_grid, 2, 6);
        Cell_AddEntity(_cell_start, SpawnPlayer(world, {2, 6}, JobType::JOB_MAGE));
    }


// #########################################################################
// MAIN WORLD UPDATE ENTRY
// #########################################################################

    void Update_World(World& world, CameraController::CameraController cam, float dt)
    {
        assert(World_FindCursor(world) >= 0 && "We need the cursor to exist as an entity");
        
        CursorSystem::Update(world);
        MovementSystem::Update(world, dt);
        CollisionSystem::Update(world);
        HoverSystem::Update(world);
        GameplaySystem::Update(world);


        // On check si le player a change de level (trouver une autre place. Collision ? Gameplay ? Level?)
        int _player = World_FindPlayer(world);
        Vector2 _playerpos = world.transform.pos[_player];
        int newLevel = World_FindLevelContaining(world, _playerpos);

        if (newLevel != -1 && newLevel != world.active_level)
        {
            world.active_level = newLevel;
        }


    }


/////////////////////////////////////////////////////////
/// HELPERS
/////////////////////////////////////////////////////////

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
