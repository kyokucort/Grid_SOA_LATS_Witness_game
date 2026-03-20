#pragma once

#include "modules/math/vector2i.hpp"

struct Cursor
{
    Vector2i cell;

    Vector2 accumulator; // important
};
