#pragma once
#include "world/world.hpp"
#include "core/archetypes.hpp"



struct SavedEntity
{
    Archetype type;
    Vector2i cell;
};

void SaveWorldToFile(World& w, const char* filename);
void ClearWorld(World& w);
void LoadWorldFromFile(World& w, const char* filename);
const char* ArchetypeToString(Archetype type);
Archetype StringToArchetype(const std::string& s);


