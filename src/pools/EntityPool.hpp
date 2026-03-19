#pragma once

#include "core/config.hpp"
#include "core/archetypes.hpp"

struct EntityPool
{
    bool alive[MAX_ENTITIES];
    int count =0;
    //EntityType type[MAX_ENTITIES];
    Archetype type[MAX_ENTITIES];
};
