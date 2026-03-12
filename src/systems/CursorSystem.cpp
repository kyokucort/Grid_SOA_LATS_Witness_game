#include "CursorSystem.hpp"
#include "SpawnSystem.hpp"
#include "world/world.hpp"
#include "raylib.h"
#include "raymath.h"
#include "modules/math/grid_math.hpp"
#include "assert.h"

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
        Grid& grid = w.loaded_levels[w.active_level].grid;
        Vector2 _old_pos = w.transform.pos[index];
        Vector2& _pos = w.transform.pos[index];
        w.collider.bounds[index].x = _pos.x - w.transform.size[index].x/2; // On bouge le collider bounds ici. Bonne idee ? 
        w.collider.bounds[index].y = _pos.y - w.transform.size[index].y/2;


        HandleClic(w, index, grid);

        if (!w.cursor.is_free[index])
        {
            HandleGridMovement(w, index, grid);
            CheckNewCell(w.cursor_cell, grid);
        }
        else
        {
            //world.transform.pos[index] = GetMousePosition();
            _pos += GetMouseDelta() * 2;
        }

        w.cursor_cell = WorldToCell(w.loaded_levels[w.active_level], _pos);
        Vector2 _limit = {grid.position.x + (grid.width * CELL_SIZE_WORLD), grid.position.y + (grid.height * CELL_SIZE_WORLD)};
        if (_pos.x < grid.position.x || _pos.x > _limit.x || _pos.y < grid.position.y || _pos.y > _limit.y)
        {
            //_pos = _old_pos;
        }
    }


    void HandleClic(World& w, int index, Grid& grid)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (!w.cursor.is_free[index])
            {
                //EnableCursor();
                //HideCursor();
                w.cursor.is_free[index] = !w.cursor.is_free[index];
                return;
            }
/*
            int p_index = WorldManager::World_FindPlayer(w);
            if (w.hover.hovered[p_index])
            {
                grid.path.clear();
                grid.path.push_back(w.cursor_cell);
                w.cursor.is_free[index] = !w.cursor.is_free[index];
            }
            */
            ///////////////////////////////////////
            /// Ici grosse fonction a etablir (vider, le path, enlever les wall...). Enfin, noeud important en tout cas
        }

    }




    void HandleGridMovement(World& world, int index, Grid& grid)
    {
        Vector2& _pos = world.transform.pos[index];
        Vector2& _anchor = world.cursor.anchor[index];
        float& _speed = world.cursor.base_speed[index];
        float& _slide_speed = world.cursor.slide_speed[index];
        world.cursor.anchor[index] = GetCloserCell(world.transform.pos[index], grid);

    //#####################################################################
    //#####################################################################

        if (GetMovementOnGrid().x == 0.0f && GetMovementOnGrid().y == 0.0f) return;
        

    //#####################################################################

        if (GetCloserCellDistance(_pos, grid) < CELL_SIZE_WORLD/16)
        {
            _pos.x += GetMovementOnGrid().x * _speed * GetFrameTime();
            _pos.y += GetMovementOnGrid().y * _speed * GetFrameTime();
        }else{
            float _tween_speed = 30.0f;
            Vector2 _tween_direction = world.cursor.anchor[index] - _pos;
            if (abs(abs(_pos.x) - abs(_anchor.x)) < abs(abs(_pos.y) - abs(_anchor.y)))
            {
                _pos.y += GetMovementOnGrid().y * _speed * GetFrameTime();
                
                if (GetMouseDelta().y > 0) 
                    _pos.y += abs(GetMovementOnGrid().x) * _slide_speed * GetFrameTime();
                else 
                    _pos.y -= abs(GetMovementOnGrid().x) * _slide_speed * GetFrameTime();

                if (abs(_tween_direction.x) > 0.1f){
                    if (_tween_direction.x > 0) _pos.x += GetFrameTime() * _tween_speed; 
                    else if (_tween_direction.x < 0) _pos.x -= GetFrameTime() * _tween_speed; 

                }
            }
            else
            {
                _pos.x += GetMovementOnGrid().x * _speed * GetFrameTime();
                if (GetMouseDelta().x > 0) 
                    _pos.x +=  abs(GetMovementOnGrid().y) * _slide_speed * GetFrameTime();
                else 
                    _pos.x -= abs(GetMovementOnGrid().y) * _slide_speed * GetFrameTime();
                
                if (abs(_tween_direction.y) > 0.1f){
                    if (_tween_direction.y > 0) _pos.y += GetFrameTime() * _tween_speed; 
                    else if (_tween_direction.y < 0) _pos.y -= GetFrameTime() * _tween_speed; 
                }
            }

        }

        //int _test_cell = GetActiveCellIndex(_pos, grid);
        Vector2i _coords = world.cursor_cell;
        //if (_test_cell >= 0)
        //{
        //    grid.cells[_test_cell].is_wall = true;
        //}

        CheckWalls(world, index, grid, _coords);



    //#####################################################################
    //#####################################################################

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


    void CheckNewCell(Vector2i cell, Grid& grid)
    {
        if (grid.path.size() <= 0) return;
        if (grid.path.back() != cell)
        {
            grid.path.push_back(cell);
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


