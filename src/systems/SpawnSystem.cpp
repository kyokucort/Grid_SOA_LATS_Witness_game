#include "SpawnSystem.hpp"
#include "assets/TextureID.hpp"
#include "modules/math/grid_math.hpp"

int CreateEntity(World& w)
{
    for (int i = 0; i < MAX_ENTITIES; i++)
    {
        if (!w.entity.alive[i])
        {
            BaseEntitySetup(w, i);
            w.entity.count++;
            return i;
        }
    }
    return -1;
}

void BaseEntitySetup(World& w, int index)
{
    
    // Entity
    w.entity.alive[index] = true;
    
    // Collider
    w.collider.has[index] = false;

    // Cursor
    w.cursor.has[index] = false;

    // Job
    w.job.has[index] = false;

    // Hover
    w.hover.has[index] = false;

    // Path
    w.path.has[index] = false;
}

void DestroyEntity(World& w, int e)
{
    w.entity.alive[e] = false;
}


int SpawnPlayer(World& world, Vector2i coords, JobType job)
{
    int e = CreateEntity(world);
    if (e == -1) return -1;

    Grid _grid = world.loaded_levels[world.active_level].grid;
    Vector2 _pos = CellCenter(coords, _grid.position, CELL_SIZE_WORLD);
    

    // Type
    world.entity.type[e] = EntityType::ENTITY_PLAYER;

    // Transform
    world.transform.pos[e] = _pos;
    world.transform.size[e] = {24, 24};
    world.transform.scale[e] = {4,4};
    world.transform.cell[e] = coords;

    //Collider
    world.collider.has[e] = true;
    world.collider.bounds[e] = {_pos.x - world.transform.size[e].x/2, _pos.y - world.transform.size[e].y/2, 32, 32};
    world.collider.is_under_cursor[e] = false;

    // Job
    world.job.has[e] = true;
    world.job.type[e] = job;

    // Hover
    world.hover.has[e] = true;

    // Path
    world.path.has[e] = true;

    // Signal
    world.signal.has[e] = true;
    world.signal.data[e].signal = Signal::SIGNAL_BLUE;

    // Modifier
    world.modifier.has[e] = true;
    world.modifier.data[e].add = Signal::SIGNAL_BLUE;

    // Render
    world.render.layer[e] = 3;
    world.render.color[e] = WHITE;
    world.render.src[e] = {0, 0, 24, 24};
    world.render.texture[e] = GetTextureFromJob(job);

    return e;
}

TextureID GetTextureFromJob(JobType job){
    if (job == JobType::JOB_MAGE){
        return TextureID::Mage;
    }
    return TextureID::NoTexture;
}

int SpawnKey(World& w, Vector2i coords)
{
    int e = CreateEntity(w);
    if (e == -1) return -1;

    Grid _grid = w.loaded_levels[w.active_level].grid;
    Vector2 _pos = CellCenter(coords, _grid.position, CELL_SIZE_WORLD);
    

    // Type
    w.entity.type[e] = EntityType::ENTITY_KEY;

    // Transform
    w.transform.pos[e] = _pos;
    w.transform.size[e] = {32, 32};
    w.transform.scale[e] = {2,2};
    w.transform.cell[e] = coords;

    //Collider
    w.collider.has[e] = true;
    w.collider.bounds[e] = {_pos.x - w.transform.size[e].x/2, _pos.y - w.transform.size[e].y/2, 32, 32};
    w.collider.is_under_cursor[e] = false;

    // Hover
    w.hover.has[e] = true;

    // Path
    w.path.has[e] = true;

    // Signal
    w.signal.has[e] = true;
    w.signal.data[e].signal = Signal::SIGNAL_KEY;

    // Render
    w.render.layer[e] = 3;
    w.render.color[e] = WHITE;
    w.render.src[e] = {0, 0, 32, 32};
    w.render.texture[e] = TextureID::Key;

    return e;
}

int SpawnDoor(World& w, Vector2i coords)
{
    int e = CreateEntity(w);
    if (e == -1) return -1;

    Grid _grid = w.loaded_levels[w.active_level].grid;
    Vector2 _pos = CellCenter(coords, _grid.position, CELL_SIZE_WORLD);
    

    // Type
    w.entity.type[e] = EntityType::ENTITY_DOOR;

    // Transform
    w.transform.pos[e] = _pos;
    w.transform.size[e] = {32, 32};
    w.transform.scale[e] = {4,4};
    w.transform.cell[e] = coords;

    //Collider
    w.collider.has[e] = true;
    w.collider.bounds[e] = {_pos.x - w.transform.size[e].x/2, _pos.y - w.transform.size[e].y/2, 32, 32};
    w.collider.is_under_cursor[e] = false;

    // Hover
    w.hover.has[e] = true;

    // Path
    w.path.has[e] = true;

    // Logic
    w.logic.has[e] = true;
    w.logic.rule[e].require = Signal::SIGNAL_KEY | Signal::SIGNAL_BLUE;
    w.logic.rule[e].forbid = 0;

    // Render
    w.render.layer[e] = 3;
    w.render.color[e] = WHITE;
    w.render.src[e] = {0, 0, 32, 32};
    w.render.texture[e] = TextureID::Door;

    return e;
}





int SpawnWall(World& world, Vector2 pos){
    int e = CreateEntity(world);
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
    int e = CreateEntity(world);
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
    int e = CreateEntity(world);
    if (e == -1) return -1;

    // Type
    world.entity.type[e] = EntityType::ENTITY_CELL_CONNECTOR;

    // Transform
    world.transform.pos[e] = pos;
    world.transform.size[e] = {32, 32};
    world.transform.scale[e] = {1,1};

    // Render
    world.render.layer[e] = 1;
    world.render.color[e] = WHITE;
    world.render.src[e] = {0, 0, 32, 32};
    world.render.texture[e] = TextureID::Cell_Connector;

    // Hover
    world.hover.has[e] = true;

    //Collider
    world.collider.has[e] = true;
    world.collider.bounds[e] = {pos.x - world.transform.size[e].x/2, pos.y - world.transform.size[e].y/2, 32, 32};

    return e;

}

int SpawnCursor(World& world, Vector2 pos){
    int e = CreateEntity(world);
    if (e == -1) return -1;

    // Type
    world.entity.type[e] = EntityType::ENTITY_CURSOR;

    // Transform
    world.transform.pos[e] = pos;
    world.transform.size[e] = {64, 64};
    world.transform.scale[e] = {1, 1};

    // Render
    world.render.layer[e] = 6;
    world.render.color[e] = WHITE;
    world.render.src[e] = {0, 0, 64, 64};
    world.render.texture[e] = TextureID::Cursor_Base;


    //Collider
    world.collider.has[e] = true;
    world.collider.bounds[e] = {pos.x - world.transform.size[e].x/2, pos.y - world.transform.size[e].y/2, 64, 64};

    //Cursor
    world.cursor.has[e] = true;

    return e;

}










