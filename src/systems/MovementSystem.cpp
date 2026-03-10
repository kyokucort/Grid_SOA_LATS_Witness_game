#include "MovementSystem.hpp"
#include "world/world.hpp"
#include "cursor/Cursor.hpp"
#include "raylib.h"

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
            else if (world.entity.type[i] == EntityType::ENTITY_CURSOR)
            {
                Update_Cursor(world, i);
            }

        }
    }
    

// Integration basique pour le moment

    //void Update_Player(Vector2& pos, float dt){
    void Update_Player(World& w, int index, float dt){
        float _step = 128;

        if (IsKeyPressed(KEY_W))
        {
            MovePlayer(w, index, {4, 4}, {0, 0});
        }
        if (IsKeyPressed(KEY_S))
        {
            MovePlayer(w, index, {4, 4}, {1, 0});
        }
        if (IsKeyPressed(KEY_D))
        {
            MovePlayer(w, index, {4, 4}, {2, 0});
        }
        if (IsKeyPressed(KEY_A))
        {
            MovePlayer(w, index, {4, 4}, {3, 0});
        }
        //if (IsKeyPressed(KEY_W)) pos.y -= _step;
        //if (IsKeyPressed(KEY_S)) pos.y += _step;
        //if (IsKeyPressed(KEY_A)) pos.x -= _step;
        //if (IsKeyPressed(KEY_D)) pos.x += _step;
    }

    void MovePlayer(World& w, int player, Vector2i old_cell, Vector2i new_cell)
    {
        Grid& grid = w.loaded_levels[w.active_level].grid;
        Cell& from = Grid_GetCell(grid, old_cell.x, old_cell.y);
        Cell& to   = Grid_GetCell(grid, new_cell.x, new_cell.y);

        if (to.is_wall) return;

        w.transform.pos[player] = to.center;
        Grid_MoveEntity(player, from, to);
    }

    void Update_Cursor(World& world, int index){
        CursorManager::Update(world, index, world.loaded_levels[world.active_level].grid);
    }

    void Grid_MoveEntity(int entity, Cell& from, Cell& to)
    {
        Cell_RemoveEntity(from, entity);
        Cell_AddEntity(to, entity);
    }

}
