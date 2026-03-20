#include "MovementSystem.hpp"
#include "world/world.hpp"
#include "raylib.h"
#include "modules/math/grid_math.hpp"

namespace MovementSystem
{
    void Update(World& world, float dt)
    {
        for (int i = 0; i < MAX_ENTITIES; i++)
        {
            if (!world.entity.alive[i])
                continue;

            if (world.entity.type[i] == Archetype::ARCH_PLAYER)
            {
                Update_Player(world, i, dt);
            }

        }
    }
    

// Integration basique pour le moment

    void Update_Player(World& w, int index, float dt)
    {
        Vector2i dir = {0,0};

        if (IsKeyPressed(KEY_W)) dir = {0,-1};
        if (IsKeyPressed(KEY_S)) dir = {0,1};
        if (IsKeyPressed(KEY_A)) dir = {-1,0};
        if (IsKeyPressed(KEY_D)) dir = {1,0};
        Vector2i current = w.transform.cell[index];
        Vector2i target  = { current.x + dir.x, current.y + dir.y };
        if (dir.x != 0 || dir.y != 0)
        {
            TryMoveEntity(w, index, target);
            //WorldManager::MoveEntity(w, index, target);
        }   
    }



    bool TryMoveEntity(World& w, int entity, Vector2i target)
    {
        Cell* cell = GridGetCell(w.global_grid, target);

        if (!cell)
            return false;

        if (cell->is_wall)
            return false;

        for (int i = 0; i < cell->count; ++i)
        {
            int other = cell->entities[i];

            if (w.collider.blocks[other])
                return false;
        }

        WorldManager::MoveEntity(w, entity, target);
        return true;
    }

}
