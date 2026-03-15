#pragma once
#include <stdint.h>

#include "world/world.hpp"

namespace LogicSystem
{
    bool CheckRule(uint32_t state, LogicRule& rule);
    void LogicSystem(World& w, uint32_t state);
}

