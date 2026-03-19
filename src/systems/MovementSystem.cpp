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
            WorldManager::MoveEntity(w, index, target);
        }   
    }


// #########################################################
// A EFFACER
//
//
//
    void MovePlayer(World& w, int player, Vector2i old_cell, Vector2i new_cell)
    {
        Grid& grid = w.loaded_levels[w.active_level].grid;
        Cell& from = Grid_GetCell(grid, old_cell.x, old_cell.y);
        Cell& to   = Grid_GetCell(grid, new_cell.x, new_cell.y);

        if (to.is_wall) return;

        w.transform.pos[player] = to.center;
        w.transform.cell[player] = new_cell;
        Grid_MoveEntity(player, from, to);
    }


    void Grid_MoveEntity(int entity, Cell& from, Cell& to)
    {
        Cell_RemoveEntity(from, entity);
        Cell_AddEntity(to, entity);
    }

}
