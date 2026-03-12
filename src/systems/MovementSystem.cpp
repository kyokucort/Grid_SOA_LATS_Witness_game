#include "MovementSystem.hpp"
#include "world/world.hpp"
#include "raylib.h"
#include "modules/math/grid_math.hpp"

namespace MovementSystem
{
    void Update(World& world, float dt)
    {
        for (int i = 0; i < world.entity.count; i++)
        {
            if (!world.entity.alive[i])
                continue;

            if (world.entity.type[i] == EntityType::ENTITY_PLAYER)
            {
                if (world.job.type[i] != JobType::JOB_MAGE) continue;
                Update_Player(world, i, dt);
                
                world.collider.bounds[i].x = world.transform.pos[i].x - world.transform.size[i].x/2; // On bouge le collider bounds ici. Bonne idee ??
                world.collider.bounds[i].y = world.transform.pos[i].y - world.transform.size[i].y/2;
            }

        }
    }
    

// Integration basique pour le moment

    void Update_Player(World& w, int index, float dt){
        Vector2i p_cell = w.transform.cell[index];

        if (IsKeyPressed(KEY_W))
        {
            MovePlayer(w, index, p_cell, p_cell + CARDINAL_DIRS[3]);
        }
        else if (IsKeyPressed(KEY_S))
        {
            MovePlayer(w, index, p_cell, p_cell + CARDINAL_DIRS[2]);
        }
        else if (IsKeyPressed(KEY_D))
        {
            MovePlayer(w, index, p_cell, p_cell + CARDINAL_DIRS[0]);
        }
        else if (IsKeyPressed(KEY_A))
        {
            MovePlayer(w, index, p_cell, p_cell + CARDINAL_DIRS[1]);
        }
    }

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
