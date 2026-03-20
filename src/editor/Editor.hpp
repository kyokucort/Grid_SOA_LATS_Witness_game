#pragma once

#include "raylib.h"
#include "world/world.hpp"
#include "camera/CameraController.hpp"
#include "editor/EditorState.hpp"
#include "editor/UI.hpp"


/*
void Editor_CreateLevel(World& world, Vector2 pos, int w, int h, float cell);
void Editor_Update(World& world, Editor& editor, CameraController::CameraController& camera_control);
void Editor_Update_Level(World& world, Editor& editor, CameraController::CameraController& camera_control);
//void Editor_Draw(World& world, Editor& editor);
void Editor_Draw_LevelEdit(World& world, Editor& editor, CameraController::CameraController& camera_control);
void Editor_Draw_Grid();
void Editor_Draw_UI(World& world, Editor& editor, CameraController::CameraController& camera_control);
void Editor_Draw_Main(World& world, Editor& editor);
int GetLevelUnderMouse(std::vector<Level> levels);
int GetLevelUnderMouse(std::vector<Level> levels, Camera2D cam);
float Snap(float value, float snap);
*/

namespace Editor
{
    Vector2i GetMouseCell(World& w, Camera2D camera);
    void Spawn(World& w, Vector2i cell);
    void Delete(World& w, Vector2i cell);
    void HandleDrag(World& w, Vector2i cell);
    void HandleClick(World& w, Vector2i cell);
    void Draw(World& w);
    void Update(World& w, UIContext& ctx, Camera2D cam);
    void Draw_UI(World& w, UIContext& ctx);
    void DrawTopBar(UIGrid& ui, UIContext& ctx);
    void DrawSidePanel(UIGrid& ui, UIContext& ctx);
    void DrawBottomBar(UIGrid& ui, UIContext& ctx);
    void DrawPalette(UIGrid& ui, UIContext& ctx, EditorState& editor);
    void EditorPaint(World& w);
}

