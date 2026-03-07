#include "CollisionSystem.hpp"

namespace CollisionSystem
{
    void Update(World& world)
    {
        world.collision_events.clear();

        for (int a = 0; a < world.entity.count; a++)
        {
            if (!world.entity.alive[a]) continue;
            if (!world.collider.has[a]) continue;

            Rectangle ra = world.collider.bounds[a];

            for (int b = a + 1; b < world.entity.count; b++)
            {
                if (!world.entity.alive[b]) continue;
                if (!world.collider.has[b]) continue;

                Rectangle rb = world.collider.bounds[b];

                if (CheckCollisionRecs(ra, rb))
                {
                    world.collision_events.push_back({a, b});
                }
            }
        }
    }
}
