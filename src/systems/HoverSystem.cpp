#include "HoverSystem.hpp"
#include "modules/math/grid_math.hpp"

namespace HoverSystem
{

    void Update(World& w)
    {
        Grid& _grid = w.global_grid;

        int _hovered = CellIndex(w.cursor_cell, _grid.width);
        bool _inside_level =  IsCellInside(w.cursor_cell, _grid.width, _grid.height);
        if (_hovered < 0 ) return;
        if (!_inside_level) return;

        for (int e = 0; e < MAX_ENTITIES; e++)
        {
            if (!w.hover.has[e]) continue; 
            w.hover.hovered[e] = false; // Reset all hovered
        }

        Cell& _cell = _grid.cells[_hovered];
        for (int e = 0; e < _cell.count; e++)
        {
            int _entity = _cell.entities[e];
            w.hover.hovered[_entity] = true;

        }


    }
}
