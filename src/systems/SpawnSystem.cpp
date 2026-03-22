#include "SpawnSystem.hpp"
#include "world/world.hpp"
#include "assets/TextureID.hpp"
#include "modules/math/grid_math.hpp"
#include "spatial/Grid.hpp"

std::array<ArchetypeBuilder, ARCH_COUNT>& GetBuilders()
{
    static std::array<ArchetypeBuilder, ARCH_COUNT> builders;
    return builders;
}


void InitSpawnSystem()
{
    GetBuilders()[ARCH_PLAYER] = BuildPlayer;
    GetBuilders()[ARCH_CURSOR] = BuildCursor;
    GetBuilders()[ARCH_KEY] = BuildKey;
    GetBuilders()[ARCH_DOOR] = BuildDoor;
    GetBuilders()[ARCH_WALL] = BuildWall;
}

int CreateFromArchetype(World& w, Archetype type, Vector2i cell)
{
    int e = CreateEntity(w);
    if (e == -1) return -1;

    w.entity.alive[e] = true;
    BaseEntitySetup(w, e);
    GetBuilders()[type](w, e, cell);

    GridInsert(w.global_grid, cell, e);

    return e;
}

void BuildPlayer(World& w, int e, Vector2i cell)
{
    Vector2 pos = CellCenter(cell, w.global_grid.origin, CELL_SIZE_WORLD);

    AddEntity(w, e, Archetype::ARCH_PLAYER);
    AddTransform(w, e, pos, {24, 24}, {4, 4}, cell);
    AddCollider(w, e, pos, {32, 32});
    //AddCollider(w, e);
    //AddJob(w, e, JOB_MAGE);
    //AddHover(w, e);
    //AddPath(w, e);

    //AddSignal(w, e, SIGNAL_BLUE);
    //AddModifier(w, e, SIGNAL_BLUE);

    AddRender(w, e, TextureID::Mage, {0, 0, 24, 24});
}

void BuildCursor(World& w, int e, Vector2i cell)
{
    Vector2 pos = CellCenter(cell, w.global_grid.origin, CELL_SIZE_WORLD);

    AddEntity(w, e, Archetype::ARCH_CURSOR);
    AddTransform(w, e, pos, {64, 64}, {1, 1}, cell);
    AddRender(w, e, TextureID::Cursor_Base, {0, 0, 64, 64});
    AddCursor(w, e);
    //AddCollider(w, e);
    //AddJob(w, e, JOB_MAGE);
    //AddHover(w, e);

    //AddSignal(w, e, SIGNAL_BLUE);
    //AddModifier(w, e, SIGNAL_BLUE);

}

void BuildKey(World& w, int e, Vector2i cell)
{
    Vector2 pos = CellCenter(cell, w.global_grid.origin, CELL_SIZE_WORLD);

    AddEntity(w, e, Archetype::ARCH_KEY);
    AddTransform(w, e, pos, {32, 32}, {2, 2}, cell);
    AddRender(w, e, TextureID::Key, {0, 0, 32, 32});
    AddPath(w, e);
    AddHover(w, e);
    AddSignal(w, e, SIGNAL_KEY);
    //AddCollider(w, e);
    //AddJob(w, e, JOB_MAGE);
    //AddPath(w, e);

    //AddModifier(w, e, SIGNAL_BLUE);

}

void BuildWall(World& w, int e, Vector2i cell)
{
    Vector2 pos = CellCenter(cell, w.global_grid.origin, CELL_SIZE_WORLD);

    AddEntity(w, e, Archetype::ARCH_WALL);
    AddTransform(w, e, pos, {64, 64}, {1, 1}, cell);
    AddRender(w, e, TextureID::Wall, {0, 0, 128, 128});
    AddCollider(w, e, pos, {32, 32});

}

void BuildDoor(World& w, int e, Vector2i cell)
{
    Vector2 pos = CellCenter(cell, w.global_grid.origin, CELL_SIZE_WORLD);

    AddEntity(w, e, Archetype::ARCH_DOOR);
    AddTransform(w, e, pos, {64, 64}, {1, 1}, cell);
    AddRender(w, e, TextureID::Door, {0, 0, 32, 32});
    AddCollider(w, e, pos, {32, 32});
    AddPath(w, e);
    AddHover(w, e);
    AddLogic(w, e);
}

void AddLogic(World& w, int e)
{
    w.logic.has[e] = true;
    w.logic.rule[e].require = Signal::SIGNAL_KEY;
    w.logic.rule[e].forbid = 0;
}


void AddEntity(World& w, int e, Archetype type)
{
    w.entity.type[e] = type;
    w.entity.alive[e] = true;
}

void AddTransform(World& w, int e, Vector2 pos, Vector2 size, Vector2 scale, Vector2i cell)
{
    w.transform.pos[e] = pos;
    w.transform.size[e] = size;
    w.transform.scale[e] = scale;
    w.transform.cell[e] = cell;
}

void AddCollider(World& w, int e, Vector2 pos, Vector2 size)
{
    w.collider.has[e] = true;
    w.collider.blocks[e] = true;
    w.collider.bounds[e] = {pos.x - size.x/2, pos.y - size.y/2, size.x, size.y};
    w.collider.is_under_cursor[e] = false;

}

void AddRender(World& w, int e, TextureID id_texture, Rectangle src)
{
    w.render.texture[e] = id_texture;
    w.render.src[e] = src;
    w.render.layer[e] = 3;
    w.render.color[e] = WHITE;
}

void AddPath(World& w, int e)
{
    w.path.has[e] = true;
}

void AddCursor(World& w, int e)
{
    w.cursor.has[e] = true;
}

void AddHover(World& w, int e)
{
    w.hover.has[e] = true;
}

void AddSignal(World& w, int e, Signal signal)
{
    w.signal.has[e] = true;
    w.signal.data[e].signal = signal;
}

//######################################
//######################################
//######################################
//######################################
//######################################
//######################################
//######################################

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


/*

int SpawnDoor(World& w, Vector2i coords)
{
    int e = CreateEntity(w);
    if (e == -1) return -1;

    Grid _grid = w.loaded_levels[w.active_level].grid;
    Vector2 _pos = CellCenter(coords, _grid.position, CELL_SIZE_WORLD);
    

    // Type
    w.entity.type[e] = Archetype::ARCH_DOOR;

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
    world.entity.type[e] = Archetype::ARCH_WALL;

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
    world.entity.type[e] = Archetype::ARCH_FLOOR_GRASS;

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
    world.entity.type[e] = Archetype::ARCH_CELL_CONNECTOR;

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










*/
