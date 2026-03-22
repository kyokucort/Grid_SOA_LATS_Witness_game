#include "world.hpp"
#include "systems/SpawnSystem.hpp"
#include "systems/CursorSystem.hpp"
#include "systems/MovementSystem.hpp"
#include "systems/CollisionSystem.hpp"
#include "systems/GameplaySystem.hpp"
#include "systems/HoverSystem.hpp"
#include "systems/LogicSystem.hpp"
#include "modules/math/grid_math.hpp"
#include "assert.h"
#include "editor/Editor.hpp"


namespace WorldManager{

    void Init_World(World& world, CameraController::CameraController& camera_control)
    {
        world.entity.count = 0;
        world.active_level = 0;
        world.active_interactible = -1;

        CameraController::Init(camera_control, SCREEN_WIDTH, SCREEN_HEIGHT);

        Init_WorldGrid(world.global_grid, 1000, 1000, CELL_SIZE_WORLD, {0, 0});
        InitSpawnSystem();
        CursorSystem::Init(world, CreateFromArchetype(world, ARCH_CURSOR, {200, 200}));
        CreateFromArchetype(world, ARCH_KEY, {2, 2});
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
        LogicSystem::LogicSystem(world, world.state);


        if (IsKeyPressed(KEY_P)){
            CreateFromArchetype(world, ARCH_PLAYER, {1, 1});
        }

    }


/////////////////////////////////////////////////////////
/// HELPERS
/////////////////////////////////////////////////////////

    int World_FindPlayer(const World& world)
    {   
        for (int i = 0; i < MAX_ENTITIES; i++){
            if (world.entity.type[i] == Archetype::ARCH_PLAYER)
                return i;
        }
        return -1;
    }

    int World_FindCursor(const World& world)
    {   
        for (int i = 0; i < MAX_ENTITIES; i++){
            if (world.entity.type[i] == Archetype::ARCH_CURSOR)
                return i;
        }
        return -1;
    }

    void MoveEntity(World& w, int e, Vector2i new_cell)
    {
        Vector2i old = w.transform.cell[e];

        if (old.x == new_cell.x && old.y == new_cell.y)
            return;

        GridRemove(w.global_grid, old, e);
        GridInsert(w.global_grid, new_cell, e);

        w.transform.cell[e] = new_cell;

        // update position world
        w.transform.pos[e] = CellCenter(new_cell, w.global_grid.origin, w.global_grid.cell_size);
        w.collider.bounds[e].x = w.transform.pos[e].x - w.transform.size[e].x/2; // On bouge le collider bounds ici. Bonne idee ??
        w.collider.bounds[e].y = w.transform.pos[e].y - w.transform.size[e].y/2;
    }
    
    void RemoveEntity(World& w, int e)
    {
        if (!w.entity.alive[e])
            return;

        // 1. retirer de la grid
        Vector2i cell = w.transform.cell[e];
        GridRemove(w.global_grid, cell, e);

        // 2. reset composants
        w.collider.has[e] = false;
        w.cursor.has[e]   = false;
        w.job.has[e]      = false;
        w.hover.has[e]    = false;
        w.path.has[e]     = false;
        w.signal.has[e]   = false;
        w.modifier.has[e] = false;

        // (optionnel) reset transform
        w.transform.pos[e] = {0,0};
        w.transform.cell[e] = {0,0};

        // 3. marquer mort
        w.entity.alive[e] = false;
        w.entity.count--;
    }
}




