#include "GameplaySystem.hpp"

namespace GameplaySystem
{

    void Update(World& world)
    {
        for (int i = 0; i < world.loaded_levels.size(); i++){
            {
                world.render.color[i] = WHITE;
            }
        }
        for (auto& ev : world.collision_events)
        {
            int a = ev.a;
            int b = ev.b;

            HandleInteraction(world, a, b);

        }
    }


    void HandleInteraction(World& world, int a, int b)
    {
        //world.cursor.hovered = -1;
        EntityType a_type = world.entity.type[a];
        EntityType b_type = world.entity.type[b];
        //world.collider.is_under_cursor[a] = false;
        //world.collider.is_under_cursor[b] = false;

        if (a_type == EntityType::ENTITY_CURSOR)
        {
            //world.cursor.hovered = b;
        }
        if (b_type == EntityType::ENTITY_CURSOR)
        {
            //world.cursor.hovered = a;
        }
            /*
            if (a_type == EntityType::ENTITY_CURSOR)
            {
                HandleCursorInteraction(world, a, b);
            }
            else if (b_type == EntityType::ENTITY_CURSOR)
            {
                HandleCursorInteraction(world, b, a);
            }
            */
    }

    void HandleCursorInteraction(World& world, int cursor, int other)
    {
        EntityType other_type = world.entity.type[other];

        if (other_type == EntityType::ENTITY_CELL_CONNECTOR)
        {
            if (world.cursor.is_free[cursor]) return;
            world.render.color[other] = RAYWHITE;
        }
        if (other_type == EntityType::ENTITY_PLAYER)
        {
            //world.render.color[other] = RED;
        }
    }



}
