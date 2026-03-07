#include "GameplaySystem.hpp"

namespace GameplaySystem
{

    void Update(World& world)
    {
        for (auto& ev : world.collision_events)
        {
            int a = ev.a;
            int b = ev.b;

            EntityType a_type = world.entity.type[a];
            EntityType b_type = world.entity.type[b];

            if (a_type == EntityType::ENTITY_CELL_CONNECTOR){
                if (b_type == EntityType::ENTITY_CURSOR){
                    world.render.color[a] = RAYWHITE;
                    printf("collision\n#########]\n\n\n");
                }
            }
            if (b_type == EntityType::ENTITY_CELL_CONNECTOR){
                if (a_type == EntityType::ENTITY_CURSOR){
                    world.render.color[b] = RAYWHITE;
                    printf("collision\n#########]\n\n\n");
                }
            }

        }
    }
}
