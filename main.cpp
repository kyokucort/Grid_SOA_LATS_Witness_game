#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "externals/gui/raygui.h"

#include <stdio.h>
#include <string>
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




//Main functions

void Init()
{
    //SetTargetFPS(60);
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "INTER LIGERE");
    InitAudioDevice();
    ToggleFullscreen();


    AssetsLoad(assets);
    WorldManager::Init_World(world, camera_control);
}

void Update()
{
    WorldManager::Update_World(world, camera_control, GetFrameTime());
    CameraController::Update(camera_control);
    CameraController::SetTarget(camera_control, world.loaded_levels[world.active_level].center);
    Editor_Update(world, editor, camera_control);

    

    // Pour debug, a supprimer
    if (IsKeyPressed(KEY_SPACE)){
        ToggleFullscreen();
    }
    if (IsKeyPressed(KEY_TWO)){
        printf("entity count is %i\n #####\n\n", world.entity.count);
        world.loaded_levels.push_back(SpawnLevel({800, 200}, 5, 9));
    }

    if (IsKeyPressed(KEY_R)){
        AssetsLoad(assets);
    }
}

void Draw()
{
    BeginDrawing();

        ClearBackground(BEIGE);
        BeginMode2D(camera_control.cam);

        RenderSystem::DrawWorld(world, assets);
        Editor_Draw(world, editor);

        EndMode2D();

        // UI Drawings

        Editor_Draw_UI(world, editor, camera_control);
        DrawFPS(SCREEN_WIDTH - 30, SCREEN_HEIGHT - 30);
        DrawText(TextFormat("%i - %i", world.cursor_cell.x, world.cursor_cell.y), 20, 700, 12, BLACK);
        DrawText(TextFormat("%.0f - %.0f", world.mouse_world.x, world.mouse_world.y), 20, 720, 12, BLACK);
        DrawText(TextFormat("%.0f - %.0f", world.transform.pos[WorldManager::World_FindCursor(world)].x, world.transform.pos[WorldManager::World_FindCursor(world)].y), 20, 750, 12, BLACK);

    EndDrawing();
}

