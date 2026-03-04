#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "modules/raygui.h"

#include <stdio.h>
#include <string>
#include "src/modules/buttons/button.hpp"
#include "src/world/world.hpp"
#include "systems/SpawnSystem.hpp"
#include "systems/RenderSystem.hpp"
#include "assets/AssetManager.hpp"
#include "editor/Editor.hpp"

// Main variables

AssetManager assets;
World world;
Editor editor;
CameraController::CameraController camera_control;



// Main entry functions

void Init();
void Update();        
void Draw();

//#####################################################################################
// Le world contient tous les elements du jeu.
// Pour le moment deux principaux types d'objets :
//      Level: contient une grid
//          Grid : contient des cells
//              Cell : contient des index vers les entites dans world
//          
//      Entite : SOA dans world
//          EntityPool : gere l'existence de l'objet.
//          TransformPool : gere la position, le size, le scale
//          ColliderPool : gere les collisions
//          RenderPool : gere les draw
//
// Level.cpp : contient la generation des levels et leur management
// 
// Seul level est specifique pour le moment, toutes les autres entites sont dans le SOA
//#####################################################################################

//------------------------------------------------------------------------------------
//
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    Init();

    while (!WindowShouldClose())
    {
        Update();
        Draw();
    }

    AssetsUnload(assets);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}

void Init()
{
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib [core] example - basic screen manager");
    InitAudioDevice();
    //SetConfigFlags(FLAG_FULLSCREEN_MODE);
    //SetTargetFPS(60);
    ToggleFullscreen();
    AssetsLoad(assets);
    Init_World(world);
    CameraInit(camera_control, SCREEN_WIDTH, SCREEN_HEIGHT);
    SpawnPlayer(world, {0, 0});
    world.loaded_levels.push_back(SpawnLevel({0, 0}, 14, 8));
    InitCursor(world.cursor, world.loaded_levels[0].grid);

    Vector2 _level_center = world.loaded_levels[world.active_level].position;
    _level_center.x += world.loaded_levels[world.active_level].grid.width * CELL_SIZE_WORLD/2;
    _level_center.y += world.loaded_levels[world.active_level].grid.height * CELL_SIZE_WORLD/2;
    CameraController::SetTarget(camera_control, _level_center);
}

void Update()
{
    Update_World(world, GetFrameTime());
    CameraUpdate(camera_control);
    Editor_Update(world, editor, camera_control);

    if (IsKeyPressed(KEY_SPACE)){
        ToggleFullscreen();
    }
    if (IsKeyPressed(KEY_TWO)){
        printf("entity count is %i\n #####\n\n", world.entity.count);
        world.loaded_levels.push_back(SpawnLevel({800, 200}, 5, 9));
    }
    UpdateCursor(world.cursor, world.loaded_levels[0].grid);

    if (editor.enabled) EnableCursor();
    else DisableCursor();
}

void Draw()
{
    BeginDrawing();

        ClearBackground(BEIGE);
        BeginMode2D(camera_control.cam);

        DrawWorld(world, assets);
        Editor_Draw(world, editor);

        DrawCursor(world);
        EndMode2D();

        //
        // UI Drawings
        //

        Editor_Draw_UI(world, editor, camera_control);
        DrawFPS(SCREEN_WIDTH - 30, SCREEN_HEIGHT - 30);

    EndDrawing();
}

