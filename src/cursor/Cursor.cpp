#include "Cursor.hpp"
#include "core/config.hpp"
#include "world/world.hpp"
#include "raymath.h"
#include "assert.h"

namespace CursorManager
{
    
    void Init(World& world, int index, Vector2 position)
    {
        assert(world.cursor.has[index] == true && "The entity has no cursor component");
        world.transform.pos[index] = position;
        world.cursor.can_turn[index] = true;
        world.cursor.base_speed[index] = CELL_SIZE_WORLD;
        world.cursor.slide_speed[index] = CELL_SIZE_WORLD/2;
        world.cursor.is_free[index] = true;
    }


    void Update(World& world, int index, Grid& grid)
    {
        //HideCursor();
        world.collider.bounds[index].x = world.transform.pos[index].x - world.transform.size[index].x/2;
        world.collider.bounds[index].y = world.transform.pos[index].y - world.transform.size[index].y/2;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (!world.cursor.is_free[index])
            {
                EnableCursor();
                HideCursor();
            }
            else 
            {
                SetMousePosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
                DisableCursor();
                grid.path.clear();
                Vector2 _cell = GetActiveCellCoords(world.transform.pos[index], grid);
                grid.path.push_back(_cell);
            }
            world.cursor.is_free[index] = !world.cursor.is_free[index];
        }
        if (!world.cursor.is_free[index])
        {
            HandleGridMovement(world, index, grid);
            CheckNewCell(world.transform.pos[index], grid);
        }
        else
        {
            world.transform.pos[index] = GetMousePosition();
            //world.transform.pos[index].x -= world.transform.size[index].x/2;
            //world.transform.pos[index].y -= world.transform.size[index].y/2;
            //world.transform.pos[index].x -= 64;
            //world.transform.pos[index].y -= 32;
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

        Vector2 _coords = GetActiveCellCoords(_pos, grid);
        int _test_cell = GetCellFromCoords(grid, _coords.x, _coords.y);
        if (_test_cell >= 0)
        {
            grid.cells[_test_cell].is_wall = true;
        }


        CheckWalls(world, index, grid, _coords);

        printf("Coords: %0.1f - %0.1f", _coords.x, _coords.y);


    //#####################################################################
    //#####################################################################

    }

    void CheckWalls(World& world, int index, Grid& grid, Vector2 coords)
    {
        int _size = grid.cells.size();
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

    Vector2 GetActiveCellCoords(Vector2 position, Grid& grid)
    {
        for (int i = 0; i < grid.cells.size(); i++){
            Vector2 _center = grid.cells[i].center;
            Rectangle _rec = {_center.x - CELL_SIZE_WORLD/2, _center.y - CELL_SIZE_WORLD/2, CELL_SIZE_WORLD, CELL_SIZE_WORLD};
            if (CheckCollisionPointRec(position, _rec))
            {
                return grid.cells[i].coords;
            }
        }
        return {-1, -1};
    }

    void CheckNewCell(Vector2 position, Grid& grid)
    {
        if (grid.path.size() <= 0) return;
        Vector2 _cell = GetActiveCellCoords(position, grid);
        //if (_cell.x == grid.path.back().x && _cell.y == grid.path.back().y) return;

        if (grid.path.back() != _cell)
        {
            grid.path.push_back(_cell);
        }
    }


    Vector2 GetCurrentCellCoords(Vector2 position, Grid& grid)
    {
        Vector2 _result = grid.cells[0].coords;
        for (int i = 0; i < grid.cells.size(); i++)
        {

            float _distance_next = Vector2Distance(position, grid.cells[i].center);
            float _distance_current = Vector2Distance(position, _result);
            if (_distance_next < _distance_current)
            {
                _result = grid.cells[i].coords;
            }
        }
        return _result;
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


