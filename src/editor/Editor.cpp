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
        }
    }



}


