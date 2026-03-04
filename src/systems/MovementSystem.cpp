#include "MovementSystem.hpp"
#include "world/world.hpp"
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
                Update_Player(world.transform.pos[i], dt);

        }
    }
    
    void Update_Player(Vector2& pos, float dt){
        float _step = 128;

        //if (IsKeyDown(KEY_D)) pos.x += speed * dt;

        if (IsKeyPressed(KEY_W)) pos.y -= _step;
        if (IsKeyPressed(KEY_S)) pos.y += _step;
        if (IsKeyPressed(KEY_A)) pos.x -= _step;
        if (IsKeyPressed(KEY_D)) pos.x += _step;
    }

}
