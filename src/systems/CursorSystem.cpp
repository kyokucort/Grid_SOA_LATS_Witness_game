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
    void Init(World& world)
    {
        int index = SpawnCursor(world, {0, 0});
        assert(world.cursor.has[index] == true && "The entity has no cursor component");
        world.transform.pos[index] = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
        world.cursor.can_turn[index] = true;
        world.cursor.base_speed[index] = CELL_SIZE_WORLD;
        world.cursor.slide_speed[index] = CELL_SIZE_WORLD/2;
        world.cursor.is_free[index] = true;
        SetMousePosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
        DisableCursor();
        HideCursor();
    }


    void Update(World& w)
    {
        int index = WorldManager::World_FindCursor(w);
        Level& level = w.loaded_levels[w.active_level];
        Grid& grid = w.loaded_levels[w.active_level].grid;
        //Vector2 _old_pos = w.transform.pos[index];
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
            Vector2i new_cell = WorldToCell(level, w.cursor.pos[index]);
            if (new_cell != w.cursor_cell)
            {
                //int p_index = WorldManager::World_FindPlayer(w);
                int p_index = w.active_interactible;
                OnCursorEnterCell(w, index, w.cursor_cell);
                Vector2i _coords = w.cursor_cell;

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
        w.cursor_cell = WorldToCell(w.loaded_levels[w.active_level], _pos);

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
                    //w.cursor.is_free[index] = !w.cursor.is_free[index];
                    w.cursor.is_free[index] = true;
                    return;
                }
                //CheckDoor(w, w.path.path[w.active_interactible].points);
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



    void CheckDoor(World& w, std::vector<Vector2i>& path)
    {

        if (path.size() < 1) return;

        printf("Path size is === %lu\n", path.size());
        Grid& grid = w.loaded_levels[w.active_level].grid; 
        int start = GetCellFromCoords(grid, path[0].x, path[0].y);
        int end = GetCellFromCoords(grid, path.back().x, path.back().y);

        if (start < 0 || end < 0) return;
        printf("NO CELLL \n\n");

        int in = HasInteractible(w, path.front());
        int out = HasInteractible(w, path.back());
        printf("In is === %i, %i\n", path.front().x, path.front().y);
        printf("Out is === %i\n", out);
        
        if (in < 0 || out < 0) return;

        printf("NO ENTITY \n\n");
        printf("In type is === %i\n", w.entity.type[in]);
        printf("Out type is === %i\n", w.entity.type[out]);

        if (w.entity.type[in] == EntityType::ENTITY_KEY && w.entity.type[out] == EntityType::ENTITY_DOOR)
        {

            printf("ON DOOR \n\n");
            w.entity.alive[in] = false;
            Cell_RemoveEntity(grid.cells[start], in);
            w.entity.alive[out] = false;
            Cell_RemoveEntity(grid.cells[end], out);
        }
    }


    int HasInteractible(World& w, Vector2i target_cell)
    {
        Grid& _grid = w.loaded_levels[w.active_level].grid; 
        int _cell = GetCellFromCoords(_grid, target_cell.x, target_cell.y);
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

        Grid& _grid = w.loaded_levels[w.active_level].grid;
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
        //int p_index = WorldManager::World_FindPlayer(w);
        int p_index = w.active_interactible;
        Grid& grid = w.loaded_levels[w.active_level].grid;

        if (!IsCellInside(new_cell, grid.width, grid.height))
            return;

        if (!CanMovePath(w.path.path[p_index].points, new_cell))
            return;

        w.cursor.cell[index] = new_cell;

        w.path.path[p_index].points.push_back(new_cell);
        w.cursor.axis[index] = CursorAxis::AXIS_NONE;
    }

    bool IsNearCellCenter(World& w, int index)
    {
        Level& level = w.loaded_levels[w.active_level];
        Grid& grid = level.grid;

        Vector2 center = CellCenter(w.cursor_cell, grid.position, CELL_SIZE_WORLD);
        Vector2 pos = w.transform.pos[index];

        float dist = Vector2Distance(pos, center);

        return dist < level.grid.cell_size * 0.15f;
    }

    //#####################################################################
    //#####################################################################
    //#####################################################################
    //#####################################################################
    //#####################################################################
    //#####################################################################
    //#####################################################################
    //#####################################################################


    //#####################################################################


    void UpdatePath(std::vector<Vector2i>& path, Vector2i next_cell)
    {
        if (CanMovePath(path, next_cell))
        {
            path.push_back(next_cell);
        }
    }


    //bool CanMovePath(Grid& grid, Vector2i next_cell)
    bool CanMovePath(std::vector<Vector2i>& path, Vector2i next_cell)
    {
        if (path.empty())
            return true;

        Vector2i last = path.back();

        int dx = abs(next_cell.x - last.x);
        int dy = abs(next_cell.y - last.y);
        //printf("LAST X is %i -- Y is %i\n###", last.x, last.y);
        //printf("NEXT X is %i -- Y is %i\n###\n\n", next_cell.x, next_cell.y);

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

        //printf("IN PATH FUNC ////////\n");
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














    void CheckWalls(World& world, int index, Grid& grid, Vector2i coords)
    {
        int _current_cell = GetCellFromCoords(grid, coords.x, coords.y);
        if (_current_cell < 0 ) return;

        int _up_cell = GetCellFromCoords(grid, coords.x, coords.y - 1);
        int _down_cell = GetCellFromCoords(grid, coords.x, coords.y + 1);
        int _left_cell = GetCellFromCoords(grid, coords.x - 1, coords.y);
        int _right_cell = GetCellFromCoords(grid, coords.x + 1, coords.y);
        
        if (grid.cells[_up_cell].is_wall == true){
            if (GetMovementOnGrid().y < 0){
                world.transform.pos[index].y = grid.cells[_current_cell].center.y;
            }
        }
        if (grid.cells[_down_cell].is_wall == true){
            if (GetMovementOnGrid().y > 0){
                world.transform.pos[index].y = grid.cells[_current_cell].center.y;
            }
        }
        if (grid.cells[_left_cell].is_wall == true){
            if (GetMovementOnGrid().x < 0){
                world.transform.pos[index].x = grid.cells[_current_cell].center.x;
            }
        }
        if (grid.cells[_right_cell].is_wall == true){
            if (GetMovementOnGrid().x > 0){
                world.transform.pos[index].x = grid.cells[_current_cell].center.x;
            }
        }


    }

    int GetActiveCellIndex(Vector2 position, Grid& grid)
    {
        for (int i = 0; i < grid.cells.size(); i++){
            Vector2 _center = grid.cells[i].center;
            Rectangle _rec = {_center.x - CELL_SIZE_WORLD/2, _center.y - CELL_SIZE_WORLD/2, CELL_SIZE_WORLD, CELL_SIZE_WORLD};
            if (CheckCollisionPointRec(position, _rec))
            {
                return i;
            }
        }
        return -1;
    }


    void CheckNewCell(std::vector<Vector2i>& path, Vector2i cell)
    {
        if (path.size() <= 0) return;
        if (path.back() != cell)
        {
            path.push_back(cell);
        }
    }




    Vector2 GetCloserCell(Vector2 position, Grid& grid)
    {
        Vector2 _result = grid.cells[0].center;
        for (int i = 0; i < grid.cells.size(); i++)
        {

            float _distance_next = Vector2Distance(position, grid.cells[i].center);
            float _distance_current = Vector2Distance(position, _result);
            if (_distance_next < _distance_current)
            {
                _result = grid.cells[i].center;
            }
        }
        return _result;
    }
    
    


    float GetCloserCellDistance(Vector2 position, Grid& grid)
    {
        float _result = 1000.0f;
        for (int i = 0; i < grid.cells.size(); i++)
        {
            float _distance_next = Vector2Distance(position, grid.cells[i].center);
            if (_distance_next < _result)
            {
                _result = _distance_next;
            }
        }
        return _result;
    }


    Vector2 GetMovementOnGrid()
    {
        Vector2 _mouse = GetMouseDelta();
        float _acceleration_coefficient;
        if (Vector2Length(_mouse) < 0.1f)
        {
            return {0, 0};
        }
        if (Vector2Length(_mouse) < 1.0f)
        {
            _acceleration_coefficient = 1.0f;
        }else{
            //_acceleration_coefficient = Vector2Length(_mouse) / 1.0f;
            _acceleration_coefficient = (Vector2Length(_mouse)/4) + 1.0f;
            //_acceleration_coefficient = 1.0f;
        }
        float x_component = _mouse.x;
        float y_component = _mouse.y;
        //if(abs(abs(x_component) - abs(y_component)) < 0.5f)  return {0, 0};

        if(abs(x_component) > abs(y_component))  ////HORIZONTAL
        {
            if (x_component > 0){
                return {1 * _acceleration_coefficient, 0};
            }else{
                return {-1 * _acceleration_coefficient, 0};
            }
        }else{
            if (y_component > 0){
                return {0, 1 * _acceleration_coefficient};
            }else{
                return {0, -1 * _acceleration_coefficient};
            }
        }
        return {0, 0};
    }
}


