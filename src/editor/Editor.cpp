#include "Editor.hpp"

#include "externals/gui/raygui.h"
#include <string>
#include <math.h>
#include "raymath.h"
#include "serialization/WorldSerializer.hpp"
#include "systems/SpawnSystem.hpp"
#include "modules/math/grid_math.hpp"

struct PaletteItem
{
    const char* name;
    Archetype type;
};

PaletteItem palette[] = {
    {"Player", ARCH_PLAYER},
    {"Key",    ARCH_KEY},
    {"Door",    ARCH_DOOR},
    {"Wall",    ARCH_WALL},
};

constexpr int PALETTE_COUNT = sizeof(palette) / sizeof(PaletteItem);


namespace Editor
{

    Vector2i GetMouseCell(World& w, Camera2D camera)
    {
        // 1. screen → world
        Vector2 mouse_screen = GetMousePosition();
        Vector2 mouse_world  = GetScreenToWorld2D(mouse_screen, camera);

        // 2. world → grid local
        Vector2 local;
        local.x = mouse_world.x - w.global_grid.origin.x;
        local.y = mouse_world.y - w.global_grid.origin.y;

        // 3. local → cell
        Vector2i cell;
        cell.x = (int)(local.x / w.global_grid.cell_size);
        cell.y = (int)(local.y / w.global_grid.cell_size);

        return cell;
    }

    void Spawn(World& w, Vector2i cell)
    {
        //int e = CreateEntity(w);
        //if (e == -1) return;

        int e = CreateFromArchetype(w, w.editor.selected_archetype, cell);
        if (e == -1) return;

        WorldManager::MoveEntity(w, e, cell);
    }

    void Delete(World& w, Vector2i cell)
    {
        Cell* c = GridGetCell(w.global_grid, cell);
        if (!c || c->count == 0) return;

        int e = c->entities[c->count - 1]; // top entity
        WorldManager::RemoveEntity(w, e);
    }

    void HandleDrag(World& w, Vector2i cell)
    {
        // Start drag
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Cell* c = GridGetCell(w.global_grid, cell);
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

    void HandleClick(World& w, Vector2i cell)
    {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            Cell* c = GridGetCell(w.global_grid, cell);
            if (!c) return;

            if (c->count == 0 && GetMousePosition().x > 300 && GetMousePosition().x < GetScreenWidth() - 300)
            {
                Spawn(w, cell);
            }
        }

        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
        {
            Delete(w, cell);
        }
    }

    void Draw(World& w)
    {
        Vector2i cell = w.editor.hovered_cell;

        if (cell.x < 0 || cell.y < 0) return;

        float size = w.global_grid.cell_size;

        DrawRectangleLines(
            w.global_grid.origin.x + cell.x * size,
            w.global_grid.origin.y + cell.y * size,
            size,
            size,
            RED
        );
    }

    void Update(World& w, UIContext& ctx, Camera2D cam)
    {
        //Vector2i cell = w.cursor_cell;
        Vector2i cell = GetMouseCell(w, cam);
        w.editor.hovered_cell = cell;

        //EditorSelectArchetype(w);

        HandleDrag(w, cell);
        HandleClick(w, cell);
        if (IsKeyPressed(KEY_P))
        {
            w.editor.paint_mode = !w.editor.paint_mode;
        }
        if (ctx.request_save)
        {
            SaveWorldToFile(w, "bijour.txt");
            ctx.request_save = false;
        }
        if (ctx.request_load)
        {
            LoadWorldFromFile(w, "bijour.txt");
            ctx.request_load = false;
        }
    }

    void DrawTopBar(UIGrid& ui, UIContext& ctx)
    {
        if (GuiButton(UIGetRect(ui, 0, 0), "Entity"))
            ctx.selected_tool = 0;

        if (GuiButton(UIGetRect(ui, 1, 0), "Signals"))
            ctx.selected_tool = 1;

        if (GuiButton(UIGetRect(ui, 2, 0), "Info"))
            ctx.selected_tool = 2;
    }

    void DrawSidePanel(UIGrid& ui, UIContext& ctx)
    {
        Rectangle panel = UIGetRectSpan(ui, 0, 6, 2, 8);
        GuiPanel(panel, "");

        switch (ctx.selected_tool)
        {
            case 0:
                GuiLabel(UIGetRect(ui, 0, 1), "Entity Mode");
                break;

            case 1:
                GuiLabel(UIGetRect(ui, 0, 1), "Signal Mode");
                break;

            case 2:
                GuiLabel(UIGetRect(ui, 0, 1), "Info Mode");
                break;
        }
    }

    void DrawBottomBar(UIGrid& ui, UIContext& ctx)
    {
        if (GuiButton(UIGetRect(ui, 13, 14), "Load"))
            ctx.request_load = true;

        if (GuiButton(UIGetRect(ui, 14, 14), "Save"))
            ctx.request_save = true;
    }

    void Draw_UI(World& w, UIContext& ctx)
    {
        UIGrid ui = CreateGrid(15, 15);
        float cell_h = ui.cell_size.y;

        GuiSetStyle(DEFAULT, TEXT_SIZE, (int)(cell_h * 0.3f));
        DrawTopBar(ui, ctx);
        DrawSidePanel(ui, ctx);
        DrawBottomBar(ui, ctx);
        if (w.editor.paint_mode)
        {
            DrawPalette(ui, ctx, w.editor);
        }
        Rectangle panel = UIGetRectSpan(ui, 0, 6, 2, 6);
        for (int e = 0; e < MAX_ENTITIES; e++)
        {
            if (!w.entity.alive[e])
                    continue;

            DrawText(TextFormat("Entity ID : %i : \nCoords %f - %f",e, w.transform.pos[e].x, w.transform.pos[e].y), 20, 500 + (e * 32), 12, BLACK);
            //GuiPanel(panel, TextFormat("Entity ID : %i : \nCoords%i - %i", w.transform.cell[e].x, w.transform.cell[e].y, e));
        }
    }


    void DrawPalette(UIGrid& ui, UIContext& ctx, EditorState& editor)
    {
        Rectangle panel = UIGetRectSpan(ui, 0, 6, 2, 6);
        GuiPanel(panel, "");

        for (int i = 0; i < PALETTE_COUNT; i++)
        {
            Rectangle r = UIGetRect(ui, 0, 1 + i);

            if (GuiButton(r, palette[i].name))
            {
                editor.selected_archetype = palette[i].type;
            }

            // highlight sélection
            if (editor.selected_archetype == palette[i].type)
            {
                DrawRectangleLinesEx(r, 2, RED);
            }
        }
    }


    void EditorPaint(World& w)
    {
        if (!w.editor.paint_mode)
            return;
        if (!IsCellInside(w.editor.hovered_cell, w.global_grid.width, w.global_grid.height))
            return;

        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            Vector2i cell = w.editor.hovered_cell;

            // check si déjà une entité (optionnel)
            Cell* c = GridGetCell(w.global_grid, cell);

            if (c && c->count == 0)
            {
                int e = CreateEntity(w);
                if (e == -1) return;

                CreateFromArchetype(w, w.editor.selected_archetype, cell);
                WorldManager::MoveEntity(w, e, cell);
            }
        }
    }


    void DrawEntityDebug(World& w)
    {
        for (int e = 0; e < MAX_ENTITIES; e++)
        {
            if (!w.entity.alive[e])
                    continue;

            DrawText(TextFormat("Entity ID : %i : \nCoords%i - %i", w.transform.cell[e].x, w.transform.cell[e].y, e), 20, 700, 12, BLACK);
            //DrawText(TextFormat("%i - %i", world.transform.cell[e].x, world.transform.cell[e].y), 20, 700, 12, BLACK);
        }
    }



}


/*
 *
je veux rendre le mode editor un plus visuel. Je pense organiser mon ecran en bloc, pour y faire appraitre des cadres selon le mode du editormode (Mode entity, Mode save, Mode info etc...). Mon ecran UI est alors une sorte de grosse GRID de 9*9 ou je peux avoir un bloc avec un text ou avec un bouton etc... enutilisant raygui.h. Voici a quoi ca ressemble. Qu'est-ce que tu en penses ?

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

   
