#pragma once
#include <stdint.h>
#include "world/world.hpp"

namespace PathSignalSystem
{
    uint32_t ComputePathSignal(World& w, std::vector<Vector2i>& path);
}
