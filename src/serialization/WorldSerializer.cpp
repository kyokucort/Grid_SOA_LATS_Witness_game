#include "WorldSerializer.hpp"
#include "systems/SpawnSystem.hpp"

#include <iostream>
#include <fstream>


ArchetypeEntry archetypes[] = {
    {ARCH_PLAYER, "PLAYER"},
    {ARCH_CURSOR, "CURSOR"},
    {ARCH_KEY,    "KEY"},
    {ARCH_WALL,    "WALL"}
};

constexpr int ARCH_ENTRY_COUNT = sizeof(archetypes) / sizeof(ArchetypeEntry);

void SaveWorldToFile(World& w, const char* filename)
{
    std::ofstream file(filename);

    for (int e = 0; e < MAX_ENTITIES; e++)
    {
        if (!w.entity.alive[e])
            continue;

        Archetype type = w.entity.type[e];
        Vector2i cell = w.transform.cell[e];
        file << ArchetypeToString(type) << " "
             << cell.x << " "
             << cell.y << "\n";
    }

    file.close();
}

void ClearWorld(World& w)
{
    for (int e = 0; e < MAX_ENTITIES; e++)
    {
        if (w.entity.alive[e])
            WorldManager::RemoveEntity(w, e);
    }
}

void LoadWorldFromFile(World& w, const char* filename)
{
    std::ifstream file(filename);

    ClearWorld(w);

    std::string type_str;
    int x, y;

    while (file >> type_str >> x >> y)
    {
        Archetype type = StringToArchetype(type_str);

        int e = CreateEntity(w);
        if (e == -1) continue;

        CreateFromArchetype(w, type, {x, y});
        WorldManager::MoveEntity(w, e, {x, y});
    }

    file.close();
}

const char* ArchetypeToString(Archetype type)
{
    for (int i = 0; i < ARCH_ENTRY_COUNT; i++)
    {
        if (archetypes[i].type == type)
            return archetypes[i].name;
    }
    return "UNKNOWN";
}

Archetype StringToArchetype(const std::string& s)
{
    for (int i = 0; i < ARCH_ENTRY_COUNT; i++)
    {
        if (s == archetypes[i].name)
            return archetypes[i].type;
    }
    return ARCH_NONE;
}
//##############################
//
//
//
//
//
//
//
///
/*
WorldData SerializeWorld(const World& world)
{
    WorldData data;

    for (const Level& lvl : world.loaded_levels)
    {
        LevelData ld;
        ld.position = lvl.position;
        ld.grid_width = lvl.grid.width;
        ld.grid_height = lvl.grid.height;
        ld.cell_size = lvl.grid.cell_size;

        data.levels.push_back(ld);
    }

    return data;
}


void DeserializeWorld(World& world, const WorldData& data)
{
    world.loaded_levels.clear();

    for (const LevelData& ld : data.levels)
    {
        Level lvl;
        lvl.position = ld.position;

        lvl.grid.width = ld.grid_width;
        lvl.grid.height = ld.grid_height;
        lvl.grid.cell_size = ld.cell_size;
        lvl.grid.position = ld.position;

        lvl.grid.cells.resize(ld.grid_width * ld.grid_height);
        InitLevel(lvl);

        world.loaded_levels.push_back(lvl);
    }
}



json SerializeWorldToJson(const World& world)
{
    json j;
    j["version"] = 1;
    j["levels"] = json::array();

    for (const Level& lvl : world.loaded_levels)
    {
        json jl;

        jl["position"] = { lvl.position.x, lvl.position.y };
        jl["grid_width"] = lvl.grid.width;
        jl["grid_height"] = lvl.grid.height;
        jl["cell_size"] = lvl.grid.cell_size;

        j["levels"].push_back(jl);
    }

    return j;
}

void SaveWorldToFile(const World& world, const std::string& path)
{
    json j = SerializeWorldToJson(world);
    std::ofstream file(path);
    file << j.dump(4); // 4 = indentation pretty print
}

WorldData LoadWorldFromFile(const std::string& path)
{
    std::ifstream file(path);
    json j;
    file >> j;

    WorldData data;

    for (auto& jl : j["levels"])
    {
        LevelData ld;

        ld.position.x = jl["position"][0];
        ld.position.y = jl["position"][1];
        ld.grid_width  = jl["grid_width"];
        ld.grid_height = jl["grid_height"];
        ld.cell_size   = jl["cell_size"];

        data.levels.push_back(ld);
    }

    return data;
}
*/
