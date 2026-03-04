#include "raylib.h"
#include "world/world.hpp"
#include "camera/CameraController.hpp"

enum class EditorMode
{
    None,
    Main,
    LevelEdit,
    PaintTile,
    PlaceEntity,
    Move,
    COUNT
};


struct Editor
{
    bool enabled;
    EditorMode mode;

    int selected_level = -1;

    bool dragging_level = false;
    int dragged_level = -1;
    Vector2 drag_offset = {0, 0};
};

void Editor_CreateLevel(World& world, Vector2 pos, int w, int h, float cell);
void Editor_Update(World& world, Editor& editor, CameraController::CameraController& camera_control);
void Editor_Update_Level(World& world, Editor& editor, CameraController::CameraController& camera_control);
void Editor_Draw(World& world, Editor& editor);
void Editor_Draw_LevelEdit(World& world, Editor& editor, CameraController::CameraController& camera_control);
void Editor_Draw_Grid();
void Editor_Draw_UI(World& world, Editor& editor, CameraController::CameraController& camera_control);
void Editor_Draw_Main(World& world, Editor& editor);
int GetLevelUnderMouse(std::vector<Level> levels);
int GetLevelUnderMouse(std::vector<Level> levels, Camera2D cam);
float Snap(float value, float snap);

