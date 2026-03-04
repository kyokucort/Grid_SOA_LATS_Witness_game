#include "level.hpp"
#include "core/config.hpp"


Level SpawnLevel(Vector2 pos, int width, int height)
{
    Level level;
    level.position = pos;
    InitGrid(level.grid, width, height, CELL_SIZE_WORLD, pos);


    level.collider_editor.x = pos.x;
    level.collider_editor.y = pos.y;
    level.collider_editor.width = level.grid.width * level.grid.cell_size;
    level.collider_editor.height = level.grid.height * level.grid.cell_size;

    return level;
}

void InitLevel(Level& lvl)
{
    //
    // Beau bordel a reformater.
    // Il faut etablir quelle data sont basiques et lesquelles peuvent etre calcule.
    // Les premieres dans lelveldata serialise
    // Les secondes calcule dans un "Init(Level lvl)" qui devra trigger quand on deserialise les data
    //
    //
    //
    //
    lvl.collider_editor.x = lvl.position.x;
    lvl.collider_editor.y = lvl.position.y;
    lvl.collider_editor.width = lvl.grid.width * lvl.grid.cell_size;
    lvl.collider_editor.height = lvl.grid.height * lvl.grid.cell_size;
    InitGrid(lvl.grid, lvl.grid.width, lvl.grid.height, lvl.grid.cell_size, lvl.position);
}

void LevelSetPosition(Level& lvl, Vector2 pos)
{
    lvl.position = pos;
    lvl.grid.position = pos;

    // collider editor suit
    lvl.collider_editor.x = pos.x;
    lvl.collider_editor.y = pos.y;
    InitGrid(lvl.grid, lvl.grid.width, lvl.grid.height, lvl.grid.cell_size, pos);
}
