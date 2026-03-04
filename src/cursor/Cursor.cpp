#include "Cursor.hpp"
#include "core/config.hpp"
#include "raymath.h"


void InitCursor(Cursor& cursor, Grid& grid)
{
    cursor.position = grid.position;
    cursor.can_turn = true;
    cursor.move_speed = CELL_SIZE_WORLD;
    cursor.slide_speed = CELL_SIZE_WORLD/2;
}


void UpdateCursor(Cursor &cursor, Grid grid)
{

    cursor.anchor = GetCloserCell(cursor.position, grid);
    




//#####################################################################
//#####################################################################

    if (GetMovementOnGrid().x == 0.0f && GetMovementOnGrid().y == 0.0f) return;
    

        

//#####################################################################

    if (GetCloserCellDistance(cursor.position, grid) < CELL_SIZE_WORLD/16)
    {
        cursor.position.x += GetMovementOnGrid().x * cursor.move_speed * GetFrameTime();
        cursor.position.y += GetMovementOnGrid().y * cursor.move_speed * GetFrameTime();
    }else{
        float _tween_speed = 30.0f;
        Vector2 _tween_direction = cursor.anchor - cursor.position;
        if (abs(abs(cursor.position.x) - abs(cursor.anchor.x)) < abs(abs(cursor.position.y) - abs(cursor.anchor.y)))
        {
            cursor.position.y += GetMovementOnGrid().y * cursor.move_speed * GetFrameTime();
            
            if (GetMouseDelta().y > 0) 
                cursor.position.y += abs(GetMovementOnGrid().x) * cursor.slide_speed * GetFrameTime();
            else 
                cursor.position.y -= abs(GetMovementOnGrid().x) * cursor.slide_speed * GetFrameTime();

            if (abs(_tween_direction.x) > 0.1f){
                if (_tween_direction.x > 0) cursor.position.x += GetFrameTime() * _tween_speed; 
                else if (_tween_direction.x < 0) cursor.position.x -= GetFrameTime() * _tween_speed; 

            }
        }
        else
        {
            cursor.position.x += GetMovementOnGrid().x * cursor.move_speed * GetFrameTime();
            if (GetMouseDelta().x > 0) 
                cursor.position.x +=  abs(GetMovementOnGrid().y) * cursor.slide_speed * GetFrameTime();
            else 
                cursor.position.x -= abs(GetMovementOnGrid().y) * cursor.slide_speed * GetFrameTime();
            
            if (abs(_tween_direction.y) > 0.1f){
                if (_tween_direction.y > 0) cursor.position.y += GetFrameTime() * _tween_speed; 
                else if (_tween_direction.y < 0) cursor.position.y -= GetFrameTime() * _tween_speed; 
            //cursor.position.y = cursor.anchor.y;
            }
        }

    }
    //SetMousePosition(cursor.position.x, cursor.position.y);



//#####################################################################
//#####################################################################

}

Vector2 GetCloserCell(Vector2 position, Grid grid)
{
    Vector2 _result = grid.position;
    for (int x = 0; x < grid.width; x++)
        for (int y = 0; y < grid.height; y++)
    {
        Vector2 _cell = {grid.position.x + (x * grid.cell_size), grid.position.y +  (y * grid.cell_size)};
        float _distance_next = Vector2Distance(position, _cell);
        float _distance_current = Vector2Distance(position, _result);
        if (_distance_next < _distance_current)
        {
            _result = _cell;
        }
    }
    return _result;
}

float GetCloserCellDistance(Vector2 position, Grid grid)
{
    float _result = 1000.0f;
    for (int x = 0; x < grid.width; x++)
        for (int y = 0; y < grid.height; y++)
    {
        Vector2 _cell = {grid.position.x + (x * grid.cell_size), grid.position.y +  (y * grid.cell_size)};
        float _distance_next = Vector2Distance(position, _cell);
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
        _acceleration_coefficient = Vector2Length(_mouse) + 1.0f;
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
