#pragma once

#include "raylib.h"
#include "world/world.hpp"
#include "camera/CameraController.hpp"
#include "editor/EditorState.hpp"


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

Vector2i GetMouseCell(World& w);
void EditorSpawn(World& w, Vector2i cell);
void EditorDelete(World& w, Vector2i cell);
void EditorHandleDrag(World& w, Vector2i cell);
void EditorHandleClick(World& w, Vector2i cell);
void EditorDraw(World& w);
void EditorUpdate(World& w);


