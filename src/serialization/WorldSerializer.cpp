#include "WorldSerializer.hpp"
#include "levels/level.hpp"

#include <iostream>
#include <fstream>

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
