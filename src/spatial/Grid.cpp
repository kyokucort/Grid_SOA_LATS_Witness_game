#include "Grid.hpp"
#include "assert.h"

void Init_WorldGrid(Grid& grid, int width, int height, float cell_size, Vector2 origin)
{
    grid.width = width;
    grid.height = height;
    grid.cell_size = cell_size;
    grid.position = origin;

    grid.cells.resize(width * height);

    for (Cell& c : grid.cells)
    {
        c.count = 0;
        c.is_wall = false;
    }
}

void Init_WorldCells(Grid& grid)
{
    grid.cells.resize(grid.width * grid.height);

    for (int y = 0; y < grid.height; y++)
    {
        for (int x = 0; x < grid.width; x++)
        {
            int i = y * grid.width + x;

            Cell& cell = grid.cells[i];

            cell.count = 0;
            cell.is_wall = false;
        }
    }
}

void InitGrid(Grid& grid, int width, int height, float cell_size, Vector2 position)
{
    grid.width = width;
    grid.height = height;
    grid.cell_size = cell_size;
    grid.position = position;
    Init_Cells(grid);
}

void Init_Cells(Grid& grid)
{
    for (int y = 0; y < grid.height; y++){
        for (int x = 0; x < grid.width; x++){
            Cell _cell;
            Vector2 _center = grid.position;
            _center.x += x * grid.cell_size + (grid.cell_size/2);
            _center.y += y * grid.cell_size + (grid.cell_size/2);
            _cell.coords = {static_cast<float>(x), static_cast<float>(y)};
            _cell.center = _center;
            _cell.is_wall = false;
            _cell.count = 0;
            grid.cells.push_back(_cell);
        }
    }
}

Cell& Grid_GetCell(Grid& grid, int x, int y)
{
    return grid.cells[y * grid.width + x];
}


void Cell_AddEntity(Cell& cell, int entity)
{
    if (cell.count >= MAX_ENTITIES_PER_CELL) return;

    cell.entities[cell.count] = entity;
    cell.count++;
}

void Cell_RemoveEntity(Cell& cell, int entity)
{
    for (int i = 0; i < cell.count; i++)
    {
        if (cell.entities[i] == entity)
        {
            cell.entities[i] = cell.entities[cell.count - 1];
            cell.count--;
            return;
        }
    }
}


int GetCellFromCoords(Grid& grid, int x, int y)
{
    for (int i= 0; i< grid.cells.size(); i++)
    {
        if (grid.cells[i].coords.x == x && grid.cells[i].coords.y == y)
        {
            return i;
        }
    }
    return -1;
}

