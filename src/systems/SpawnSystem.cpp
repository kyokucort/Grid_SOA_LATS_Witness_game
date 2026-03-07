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
    world.render.color[e] = WHITE;
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
    world.render.color[e] = WHITE;
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
    world.render.color[e] = WHITE;
    world.render.src[e] = {0, 0, 128, 128};
    world.render.texture[e] = TextureID::Floor_Grass;

    return e;

}


int SpawnCellConnector(World& world, Vector2 pos){
    int e = CreateEntity(world.entity);
    if (e == -1) return -1;

    // Type
    world.entity.type[e] = EntityType::ENTITY_CELL_CONNECTOR;

    // Transform
    world.transform.pos[e] = pos;
    world.transform.size[e] = {32, 32};
    world.transform.scale[e] = {1,1};

    // Render
    world.render.layer[e] = 1;
    world.render.color[e] = GRAY;
    world.render.src[e] = {0, 0, 32, 32};
    world.render.texture[e] = TextureID::Cell_Connector;

    //Collider
    world.collider.has[e] = true;
    world.collider.bounds[e] = {pos.x - world.transform.size[e].x/2, pos.y - world.transform.size[e].y/2, 32, 32};

    return e;

}

int SpawnCursor(World& world, Vector2 pos){
    int e = CreateEntity(world.entity);
    if (e == -1) return -1;

    // Type
    world.entity.type[e] = EntityType::ENTITY_CURSOR;

    // Transform
    world.transform.pos[e] = pos;
    world.transform.size[e] = {64, 64};
    world.transform.scale[e] = {1,1};

    // Render
    world.render.layer[e] = 6;
    world.render.color[e] = WHITE;
    world.render.src[e] = {0, 0, 64, 64};
    world.render.texture[e] = TextureID::Cursor_Base;


    //Collider
    world.collider.has[e] = true;
    world.collider.bounds[e] = {pos.x - world.transform.size[e].x/2, pos.y - world.transform.size[e].y/2, 64, 64};

    return e;

}










