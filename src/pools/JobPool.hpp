#pragma once

#include "raylib.h"
#include "core/config.hpp"
#include "core/job_types.hpp"

struct JobPool
{
    bool has[MAX_ENTITIES];
    JobType type[MAX_ENTITIES];
};
