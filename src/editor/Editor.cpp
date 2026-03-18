#include "Editor.hpp"

#include "externals/gui/raygui.h"
#include <string>
#include <math.h>
#include "raymath.h"
#include "serialization/WorldSerializer.hpp"
#include "systems/SpawnSystem.hpp"

constexpr Vector2 BOX = {192, 108};
constexpr Vector2 MARGIN = {96, 54};

Vector2 mouse_world;
bool mouse_dragging = false;


Vector2 cell = {-1, 1};


Vector2 base_label = {300, 100};
Vector2 base_padding = {20, 20};
std::string txt_current_mode = "NONE";


int selected_level = -1;
/*
Rectangle GetRectangleFromCoords(int x, int y)
{
    Rectangle _rec;
    _rec.x = MARGIN.x + (BOX.x * x);
    _rec.y = MARGIN.y + (BOX.y * y);
    _rec.width = BOX.x;
    _rec.height = BOX.y;

    return _rec;
}


void Editor_CreateLevel(World& world, Vector2 pos, int w, int h, float cell)
{
    Level lvl;

    lvl.position = pos;

    lvl.grid.width = w;
    lvl.grid.height = h;
    lvl.grid.cell_size = cell;
    lvl.grid.position = pos;

    lvl.grid.cells.resize(w*h);

    lvl.collider_editor.x = pos.x;
    lvl.collider_editor.y = pos.y;
    lvl.collider_editor.width = lvl.grid.width * lvl.grid.cell_size;
    lvl.collider_editor.height = lvl.grid.height * lvl.grid.cell_size;
    lvl.collider_color = WHITE;

    world.loaded_levels.push_back(std::move(lvl));
}


void Editor_Update(World& world, Editor& editor, CameraController::CameraController& camera_control)
{
    GuiSetStyle(DEFAULT, TEXT_SIZE, GuiGetFont().baseSize*2);
    if (IsKeyPressed(KEY_TAB)){
        editor.enabled = !editor.enabled;
        editor.mode = EditorMode::Main;
    }
    if(!editor.enabled) return;

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        mouse_dragging = true;
    }
    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
        mouse_dragging = false;
    }

    mouse_world.x = GetMousePosition().x + camera_control.cam.target.x - camera_control.cam.offset.x;
    mouse_world.y = GetMousePosition().y + camera_control.cam.target.y - camera_control.cam.offset.y;

    if(editor.mode == EditorMode::LevelEdit)
    {
        Editor_Update_Level(world, editor, camera_control);
    }

}

void Editor_Update_Level(World& world, Editor& editor, CameraController::CameraController& camera_control)
{
    for (int i=0; i < world.loaded_levels.size(); i++)
    {
        world.loaded_levels[i].collider_color = WHITE;
    }
            
    int _hovered_lvl = GetLevelUnderMouse(world.loaded_levels, camera_control.cam);
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && _hovered_lvl >= 0)
    {
        editor.dragging_level = true;
        editor.dragged_level = _hovered_lvl;

        Vector2 mouse_world = GetScreenToWorld2D(GetMousePosition(), camera_control.cam);
        editor.drag_offset = Vector2Subtract(mouse_world, world.loaded_levels[_hovered_lvl].position);
    }
    if (editor.dragging_level)
    {
        Vector2 mouse_world = GetScreenToWorld2D(GetMousePosition(), camera_control.cam);

        Vector2 target = Vector2Subtract(mouse_world, editor.drag_offset);

        // snap
        target.x = Snap(target.x, CELL_SIZE_WORLD);
        target.y = Snap(target.y, CELL_SIZE_WORLD);

        LevelSetPosition(world.loaded_levels[editor.dragged_level], target);
    }

    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        editor.dragging_level = false;
        editor.dragged_level = -1;
    }

}

void Editor_Draw(World& world, Editor& editor)
{
    if(!editor.enabled) return;
    Editor_Draw_Grid();
    if(editor.mode == EditorMode::LevelEdit)
    {
        for (int i=0; i < world.loaded_levels.size(); i++)
        {
            DrawRectangleRec(world.loaded_levels[i].collider_editor, Fade(world.loaded_levels[i].collider_color, 0.5f));
        }
    }

}

void Editor_Draw_Grid()
{
    for (int x=-50; x < 50; x++){
        for (int y=-50; y < 50; y++){
            Rectangle _cell;
            _cell.x = x * CELL_SIZE_WORLD;  // Add offset for centered ?
            _cell.y = y * CELL_SIZE_WORLD;  // Add offset for centered ?
            _cell.width = CELL_SIZE_WORLD;
            _cell.height = CELL_SIZE_WORLD;
            DrawRectangleLinesEx(_cell, 1.0f, Fade(WHITE, 0.5f));
            DrawText(TextFormat("%i - %i",x, y), _cell.x, _cell.y, 10, BLACK);
        }
    }
}

void Editor_Draw_Main(World& world, Editor& editor)
{
    if(GuiButton(GetRectangleFromCoords(0, 0), "Level Mode"))
    {
        editor.mode = EditorMode::LevelEdit;
    }
    if(GuiButton(GetRectangleFromCoords(8, 8), "Save World"))
    {
        SaveWorldToFile(world, "level.txt");
    }
    if(GuiButton(GetRectangleFromCoords(6, 8), "Load World"))
    {
        DeserializeWorld(world, LoadWorldFromFile("level.txt"));
    }
}

void Editor_Draw_LevelEdit(World& world, Editor& editor, CameraController::CameraController& camera_control)
{
    if(GuiButton(GetRectangleFromCoords(0, 8), "New Level"))
    {
        //Editor_CreateLevel(world, {100,0}, 32, 32, 32);
        Editor_CreateLevel(world, camera_control.cam.target, 32, 32, 32);
        editor.selected_level = world.loaded_levels.size()-1;
    }
    GuiDummyRec(GetRectangleFromCoords(8, 0), "Editor Mode");
    GuiDummyRec(GetRectangleFromCoords(8, 1), TextFormat("Current mode :\n %s", txt_current_mode.c_str()));
    GuiDummyRec(GetRectangleFromCoords(8, 4), TextFormat("Lvl total : %i", world.loaded_levels.size()));
    GuiDummyRec(GetRectangleFromCoords(8, 5), TextFormat("Mouse Pos: {%f, %f}", GetMousePosition().x, GetMousePosition().y));
}


void Editor_Draw_UI(World& world, Editor& editor, CameraController::CameraController& camera_control)
{
    if(!editor.enabled) return;
    if(editor.mode == EditorMode::Main)
    {
        Editor_Draw_Main(world, editor);
    }
    if(editor.mode == EditorMode::LevelEdit)
    {
        Editor_Draw_LevelEdit(world, editor, camera_control);
    }
}

int GetLevelUnderMouse(std::vector<Level> levels, Camera2D cam)
{
    Vector2 _target_pos = GetScreenToWorld2D(GetMousePosition(), cam);
    for (int i=0; i < levels.size(); i++){
        if (CheckCollisionPointRec(_target_pos, levels[i].collider_editor)){
            return i;
        }
    }
    return -1;
}


float Snap(float value, float snap)
{
    value = roundf(value / snap) * snap;
    return value;
}

*/

//##################################
//
//
//

Vector2i GetMouseCell(World& w)
{
    Vector2 mouse = GetMousePosition();

    Vector2 local;
    local.x = mouse.x - w.global_grid.position.x;
    local.y = mouse.y - w.global_grid.position.y;

    Vector2i cell;
    cell.x = (int)(local.x / w.global_grid.cell_size);
    cell.y = (int)(local.y / w.global_grid.cell_size);

    return cell;
}


void EditorSpawn(World& w, Vector2i cell)
{
    //int e = CreateEntity(w);
    //if (e == -1) return;

    int e = CreateFromArchetype(w, w.editor.selected_archetype, cell);
    if (e == -1) return;

    WorldManager::MoveEntity(w, e, cell);
}

void EditorDelete(World& w, Vector2i cell)
{
    Cell* c = GetCell(w.global_grid, cell.x, cell.y);
    if (!c || c->count == 0) return;

    int e = c->entities[c->count - 1]; // top entity
    WorldManager::RemoveEntity(w, e);
}

void EditorHandleDrag(World& w, Vector2i cell)
{
    // Start drag
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Cell* c = GetCell(w.global_grid, cell.x, cell.y);
        if (c && c->count > 0)
        {
            w.editor.selected_entity = c->entities[c->count - 1];
        }
    }

    // End drag
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
    {
        if (w.editor.selected_entity != -1)
        {
            WorldManager::MoveEntity(w, w.editor.selected_entity, cell);
            w.editor.selected_entity = -1;
        }
    }
}

void EditorHandleClick(World& w, Vector2i cell)
{
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        Cell* c = GetCell(w.global_grid, cell.x, cell.y);

        if (!c || c->count == 0)
        {
            EditorSpawn(w, cell);
        }
    }

    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
    {
        EditorDelete(w, cell);
    }
}

void EditorDraw(World& w)
{
    Vector2i cell = w.editor.hovered_cell;

    if (cell.x < 0 || cell.y < 0) return;

    float size = w.global_grid.cell_size;

    DrawRectangleLines(
        w.global_grid.position.x + cell.x * size,
        w.global_grid.position.y + cell.y * size,
        size,
        size,
        RED
    );
}

void EditorUpdate(World& w)
{
    Vector2i cell = w.cursor_cell;
    w.editor.hovered_cell = cell;

    //EditorSelectArchetype(w);

    EditorHandleDrag(w, cell);
    EditorHandleClick(w, cell);
}
