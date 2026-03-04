#pragma once
#include "world/world.hpp"
#include "externals/json/json.hpp"


using json = nlohmann::json;

struct LevelData
{
    Vector2 position;
    int grid_width;
    int grid_height;
    float cell_size;
};


struct WorldData
{
    std::vector<LevelData> levels;
};


void DeserializeWorld(World& world, const WorldData& data);
json SerializeWorldToJson(const World& world);
void SaveWorldToFile(const World& world, const std::string& path);
WorldData LoadWorldFromFile(const std::string& path);



