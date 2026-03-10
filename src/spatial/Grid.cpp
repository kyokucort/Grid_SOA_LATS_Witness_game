#include "Grid.hpp"
#include "assert.h"


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
    for (int x = 0; x < grid.width; x++){
        for (int y = 0; y < grid.height; y++){
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


void CellInsertEntity(Grid& grid, int entity, Vector2 coords)
{
    int _cell_id = GetCellFromCoords(grid, coords.x, coords.y);
    assert(_cell_id >= 0 && "No cell on coords");
    Cell& _cell = grid.cells[_cell_id];

    _cell.entities[_cell.count] = entity;

    _cell.count += 1;
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
