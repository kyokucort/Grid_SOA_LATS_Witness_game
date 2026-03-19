#pragma once

enum Archetype
{
    ARCH_PLAYER,
    ARCH_CURSOR,
    ARCH_KEY,
    ARCH_NONE,
    ARCH_COUNT
};

struct ArchetypeEntry
{
    Archetype type;
    const char* name;
};


