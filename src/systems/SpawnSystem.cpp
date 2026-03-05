#include "SpawnSystem.hpp"
#include "assets/TextureID.hpp"

int CreateEntity(EntityPool& pool)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        if (!pool.alive[i])
        {
            pool.alive[i] = true;
            pool.count++;
            return i;
        }
    }
    return -1;
}

void DestroyEntity(World& w, int e)
{
    w.entity.alive[e] = false;
}


int SpawnPlayer(World& world, Vector2 pos)
{
    int e = CreateEntity(world.entity);
    if (e == -1) return -1;

    // Type
    world.entity.type[e] = EntityType::ENTITY_PLAYER;

    // Transform
    world.transform.pos[e] = pos;
    world.transform.size[e] = {24, 24};
    world.transform.scale[e] = {4,4};

    // Render
    world.render.layer[e] = 3;
    world.render.color[e] = RED;
    world.render.src[e] = {0, 0, 24, 24};
    world.render.texture[e] = TextureID::Player;

    return e;
}

int SpawnWall(World& world, Vector2 pos){
    int e = CreateEntity(world.entity);
    if (e == -1) return -1;

    // Type
    world.entity.type[e] = EntityType::ENTITY_WALL;

    // Transform
    world.transform.pos[e] = pos;
    world.transform.size[e] = {128, 128};
    world.transform.scale[e] = {1,1};

    // Render
    world.render.layer[e] = 2;
    world.render.color[e] = RED;
    world.render.src[e] = {0, 0, 128, 128};
    world.render.texture[e] = TextureID::Wall;

    return e;

}


int SpawnFloorGrass(World& world, Vector2 pos){
    int e = CreateEntity(world.entity);
    if (e == -1) return -1;

    // Type
    world.entity.type[e] = EntityType::ENTITY_FLOOR_GRASS;

    // Transform
    world.transform.pos[e] = pos;
    world.transform.size[e] = {128, 128};
    world.transform.scale[e] = {1,1};

    // Render
    world.render.layer[e] = 1;
    world.render.color[e] = RED;
    world.render.src[e] = {0, 0, 128, 128};
    world.render.texture[e] = TextureID::Floor_Grass;

    return e;

}













