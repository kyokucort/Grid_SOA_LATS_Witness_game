#include "CursorSystem.hpp"
#include "PathSignalSystem.hpp"
#include "SpawnSystem.hpp"
#include "world/world.hpp"
#include "raylib.h"
#include "raymath.h"
#include "modules/math/grid_math.hpp"
#include "assert.h"
#include "pools/CursorPool.hpp"



namespace CursorSystem
{
    void Init(World& world, int index)
    {
        assert(world.cursor.has[index] == true && "The entity has no cursor component");
        world.transform.pos[index] = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
        world.cursor.is_free[index] = true;
        SetMousePosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        DisableCursor();
        HideCursor();
    }


    void Update(World& w)
    {
        int index = WorldManager::World_FindCursor(w);
        assert(index >= 0 && "The entity has no cursor component");
        Grid& grid = w.global_grid;
        Vector2& _pos = w.transform.pos[index];
        w.collider.bounds[index].x = _pos.x - w.transform.size[index].x/2; // On bouge le collider bounds ici. Bonne idee ? 
        w.collider.bounds[index].y = _pos.y - w.transform.size[index].y/2;


        HandleClic(w, index, grid);

        if (!w.cursor.is_free[index])
        {
            Vector2 delta = GetMouseDelta();

            if (w.cursor.axis[index] == CursorAxis::AXIS_NONE)
            {
                if (fabs(delta.x) > fabs(delta.y)){
                    w.cursor.axis[index] = CursorAxis::AXIS_HORIZONTAL;
                }

                else if (fabs(delta.x) < fabs(delta.y)){
                    w.cursor.axis[index] = CursorAxis::AXIS_VERTICAL;
                }
            }

            UpdateCursorPhysics(w, index, GetMouseDelta(), GetFrameTime());
            Vector2i new_cell = WorldToCell(w.cursor.pos[index]);
            if (new_cell != w.cursor_cell)
            {
                int p_index = w.active_interactible;
                OnCursorEnterCell(w, index, w.cursor_cell);

                if (w.path.path[p_index].points.empty())
                {
                    w.path.path[p_index].points.push_back(w.cursor_cell);
                    return;
                }
                UpdatePath(w.path.path[p_index].points, w.cursor_cell);
            }

            if (IsNearCellCenter(w, index))
            {
                if (Vector2Length(GetMouseDelta()) > 0.5f) return;
                w.cursor.axis[index] = CursorAxis::AXIS_NONE;
            }
            SetMousePosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        }
        else
        {
            _pos += GetMouseDelta() * 2;
        }
        w.cursor_cell = WorldToCell(_pos);

    }


    void HandleClic(World& w, int index, Grid& grid)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (!w.cursor.is_free[index])
            {
                int _current = HasInteractible(w, w.cursor_cell);
                if (_current < 0)
                {
                    w.path.path[w.active_interactible].points.clear();
                    w.active_interactible = -1;
                    w.cursor.is_free[index] = true;
                    return;
                }
                w.state = PathSignalSystem::ComputePathSignal(w, w.path.path[w.active_interactible].points);
                w.cursor.is_free[index] = true;
                return;
            }
            w.active_interactible = HasInteractible(w, w.cursor_cell);
            if (w.active_interactible < 0) return;

            w.path.path[w.active_interactible].points.clear();
            w.path.path[w.active_interactible].points.push_back(w.cursor_cell);
            w.cursor.is_free[index] = false;
        }

    }

    int HasInteractible(World& w, Vector2i target_cell)
    {
        Grid& _grid = w.global_grid; 
        int _cell = CellIndex(target_cell, _grid.width);
        if (_cell < 0) return -1;

        for (int c = 0; c < _grid.cells[_cell].count; c++)
        {
            int e = _grid.cells[_cell].entities[c];
            if (!w.path.has[e]) continue;
            //if (w.hover.hovered[e])
            //{
                return e;
            //}
        }
        return -1;
    }




    void UpdateCursorPhysics(World& w, int index, Vector2 mouse_delta, float dt)
    {
        float accel = 4000.0f;
        float friction = 20.0f;

        if (w.cursor.axis[index] == CursorAxis::AXIS_HORIZONTAL)
        {
            w.cursor.velocity[index].x += mouse_delta.x * accel * dt;
            w.cursor.velocity[index].x -= w.cursor.velocity[index].x * friction * dt;
            w.transform.pos[index].x += w.cursor.velocity[index].x * dt;
        }
        else if (w.cursor.axis[index] == CursorAxis::AXIS_VERTICAL)
        {
            w.cursor.velocity[index].y += mouse_delta.y * accel * dt;
            w.cursor.velocity[index].y -= w.cursor.velocity[index].y * friction * dt;
            w.transform.pos[index].y += w.cursor.velocity[index].y * dt;
        }

        Grid& _grid = w.global_grid;
        Vector2 center = CellCenter(w.cursor_cell, _grid.position, CELL_SIZE_WORLD);

        if (w.cursor.axis[index] == CursorAxis::AXIS_HORIZONTAL)
        {
            //if (!IsNearCellCenter(w, index)) return;
            w.transform.pos[index].y = center.y;
        }

        if (w.cursor.axis[index] == CursorAxis::AXIS_VERTICAL)
        {
            //if (!IsNearCellCenter(w, index)) return;
            w.transform.pos[index].x = center.x;
        }

    }


    void OnCursorEnterCell(World& w, int index, Vector2i new_cell)
    {
        int _active = w.active_interactible;
        assert(_active >= 0 && "No active interactible");
        Grid& grid = w.global_grid;

        if (!IsCellInside(new_cell, grid.width, grid.height))
            return;

        if (!CanMovePath(w.path.path[_active].points, new_cell))
            return;

        w.cursor.cell[index] = new_cell;
        w.path.path[_active].points.push_back(new_cell);
        w.cursor.axis[index] = CursorAxis::AXIS_NONE;
    }

    bool IsNearCellCenter(World& w, int index)
    {
        Grid& grid = w.global_grid;

        Vector2 center = CellCenter(w.cursor_cell, grid.position, CELL_SIZE_WORLD);
        Vector2 pos = w.transform.pos[index];

        float dist = Vector2Distance(pos, center);

        return dist < grid.cell_size * 0.15f;
    }

    void UpdatePath(std::vector<Vector2i>& path, Vector2i next_cell)
    {
        if (CanMovePath(path, next_cell))
        {
            path.push_back(next_cell);
        }
    }

    bool CanMovePath(std::vector<Vector2i>& path, Vector2i next_cell)
    {
        if (path.empty())
            return true;

        Vector2i last = path.back();

        int dx = abs(next_cell.x - last.x);
        int dy = abs(next_cell.y - last.y);

        // mouvement cardinal uniquement
        if (dx + dy != 1)
            return false;

        // retour arrière
        if (path.size() > 1)
        {
            Vector2i prev = path[path.size() - 2];

            if (next_cell == prev)
            {
                path.pop_back();
                return false;
            }
        }

        // cellule déjà visitée
        if (PathContains(path, next_cell))
            return false;

        return true;
    }

    bool PathContains(const std::vector<Vector2i>& path, Vector2i cell)
    {
        for (const Vector2i& c : path)
        {
            if (c == cell)
                return true;
        }

        return false;
    }


}


