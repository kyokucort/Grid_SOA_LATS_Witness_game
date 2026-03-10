#include "HoverSystem.hpp"

namespace HoverSystem
{

    void Update(World& world)
    {
        int _hovered = world.active_cell;
        if (_hovered < 0 ) return;
        for (int e = 0; e < MAX_ENTITIES; e++)
        {
            if (world.hover.has[e]) continue; 
            world.hover.hovered[e] = false; // Reset all hovered
        }

        Grid& _grid = world.loaded_levels[world.active_level].grid;
        Cell& _cell = _grid.cells[_hovered];

        for (int e = 0; e < _cell.count; e++)
        {
            world.hover.hovered[_cell.entities[e]] = true;

        }


    }
}
