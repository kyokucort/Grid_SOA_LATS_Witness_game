#include "Grid.hpp"


void InitGrid(Grid& grid, int width, int height, float cell_size, Vector2 position)
{
    grid.width = width;
    grid.height = height;
    grid.cell_size = cell_size;
    grid.position = position;
}


void GridInsert(Grid& grid, int entity, Vector2 pos)
{
    int cx = pos.x / grid.cell_size;
    int cy = pos.y / grid.cell_size;
    Cell& c = grid.cells[cy*grid.width+cx];

    c.entities[c.count++] = entity;
}
