#include "GameplaySystem.hpp"

namespace GameplaySystem
{

    void Update(World& world)
    {
        for (auto& ev : world.collision_events)
        {
            int a = ev.a;
            int b = ev.b;

            HandleInteraction(world, a, b);

        }
    }


    void HandleInteraction(World& world, int a, int b)
    {
        EntityType a_type = world.entity.type[a];
        EntityType b_type = world.entity.type[b];

        if (a_type == EntityType::ENTITY_CURSOR)
        {
            HandleCursorInteraction(world, a, b);
        }
        else if (b_type == EntityType::ENTITY_CURSOR)
        {
            HandleCursorInteraction(world, b, a);
        }
    }

    void HandleCursorInteraction(World& world, int cursor, int other)
    {
        EntityType other_type = world.entity.type[other];

        if (other_type == EntityType::ENTITY_CELL_CONNECTOR)
        {
            if (world.cursor.is_free[cursor]) return;
            //world.render.color[other] = RAYWHITE;
        }
    }



}
