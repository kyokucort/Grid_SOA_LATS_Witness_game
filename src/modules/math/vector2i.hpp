#pragma once

struct Vector2i
{
    int x;
    int y;

    Vector2i() : x(0), y(0) {}
    Vector2i(int x_, int y_) : x(x_), y(y_) {}

    bool operator==(const Vector2i& other) const
    {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Vector2i& other) const
    {
        return !(*this == other);
    }

    Vector2i operator+(const Vector2i& other) const
    {
        return {x + other.x, y + other.y};
    }

    Vector2i operator-(const Vector2i& other) const
    {
        return {x - other.x, y - other.y};
    }
};
