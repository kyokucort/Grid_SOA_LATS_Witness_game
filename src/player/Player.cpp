#include "Player.hpp"
#include "core/config.hpp"
#include "raymath.h"

void Update_Player(int index)
{
    if (IsKeyPressed(KEY_A)){
        world.move.direction[index] = {-1, 0};
        MovePlayer(index);
    }
    else if (IsKeyPressed(KEY_D)){
        world.move.direction[index] = {1, 0};
        MovePlayer(world, index);
    }
    else if (IsKeyPressed(KEY_W)){
        world.move.direction[index] = {0, -1};
        MovePlayer(world, index);
    }
    else if (IsKeyPressed(KEY_S)){
        world.move.direction[index] = {0, 1};
        MovePlayer(world, index);
    }
    
}

void MovePlayer(MovePool& move, TransformPool& transform, int index)
{
    transform.pos[index] += Vector2Scale(move.direction[index], CELL_SIZE_WORLD);
    move.direction[index] = {0, 0};
}
