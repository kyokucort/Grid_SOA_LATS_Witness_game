#include "Cursor.hpp"
#include "core/config.hpp"
#include "world/world.hpp"
#include "raymath.h"

namespace CursorManager
{
    // Bordel ici. en train de reformarter pour que le cursor soit une entity
    // Question : les valeurs specifiques au cursor (typiquement slide speed), dois-je les integrer dans world directement
    // world.cursor_slide_speed
    // ou
    // les integrer dans la movepool (mais donc beaucoup de memoire pour un seul element / avantage : s'integre mieux au design general
    // Essayer d'anticiper les futurs interactions
    // Pour le moment probaablement mieux pool (beaucoup plkus scalable
    

    
    void Init(World& world, int index, Vector2 position)
    {
        world.transform.pos[index] = position;
        world.move.cursor_can_turn[index] = true;
        world.move.cursor_base_speed[index] = CELL_SIZE_WORLD;
        world.move.cursor_slide_speed[index] = CELL_SIZE_WORLD/2;
        world.move.cursor_is_free[index] = true;
        //HideCursor();
    }


    /*void Init(Cursor& cursor, Grid& grid)
    {
        cursor.position = grid.position;
        cursor.can_turn = true;
        cursor.move_speed = CELL_SIZE_WORLD;
        cursor.slide_speed = CELL_SIZE_WORLD/2;
        cursor.is_free = true;
        //HideCursor();
    }
    */

    void Update(World& world, int index, Grid& grid)
    {
        //HideCursor();
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (!world.move.cursor_is_free[index])
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
            world.move.cursor_is_free[index] = !world.move.cursor_is_free[index];
        }
        if (!world.move.cursor_is_free[index])
        {
            HandleGridMovement(world, index, grid);
            CheckNewCell(world.transform.pos[index], grid);
        }
        else
        {
            world.transform.pos[index] = GetMousePosition();
        }
    }






    void HandleGridMovement(World& world, int index, Grid& grid)
    {
        Vector2& _pos = world.transform.pos[index];
        Vector2& _anchor = world.move.anchor[index];
        float& _speed = world.move.cursor_base_speed[index];
        float& _slide_speed = world.move.cursor_slide_speed[index];
        world.move.anchor[index] = GetCloserCell(world.transform.pos[index], grid);

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
            Vector2 _tween_direction = world.move.anchor[index] - _pos;
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

        world.collider.bounds[index].x = _pos.x - world.transform.size[index].x/2;
        world.collider.bounds[index].y = _pos.y - world.transform.size[index].y/2;
        Vector2 _coords = GetActiveCellCoords(_pos, grid);
        printf("Coords: %0.1f - %0.1f", _coords.x, _coords.y);


    //#####################################################################
    //#####################################################################

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


