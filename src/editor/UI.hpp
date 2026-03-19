#pragma once
#include "raylib.h"

struct UIContext
{
    bool request_save = false;
    bool request_load = false;

    int selected_tool = 0;
};

struct UIGrid
{
    Vector2 origin = {0, 0};
    Vector2 cell_size = {0, 0};
    Vector2 padding = {4, 4};

    int width = 1;
    int height = 1;
};

inline UIGrid CreateGrid(int w, int h)
{
    UIGrid g;
    g.width = w;
    g.height = h;

    g.cell_size = {
        GetScreenWidth() / (float)w,
        GetScreenHeight() / (float)h
    };

    return g;
}


inline Rectangle UIGetRect(UIGrid& g, int x, int y)
{
    return {
        g.origin.x + x * g.cell_size.x + g.padding.x,
        g.origin.y + y * g.cell_size.y + g.padding.y,
        g.cell_size.x - 2 * g.padding.x,
        g.cell_size.y - 2 * g.padding.y
    };
}

inline Rectangle UIGetRectSpan(UIGrid& g, int x, int y, int w, int h)
{
    return {
        g.origin.x + x * g.cell_size.x + g.padding.x,
        g.origin.y + y * g.cell_size.y + g.padding.y,
        g.cell_size.x * w - 2 * g.padding.x,
        g.cell_size.y * h - 2 * g.padding.y
    };
}
