#pragma once

#include "core/archetypes.hpp"

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

struct EditorState
{
    bool enabled;
    EditorMode mode;

    int selected_level = -1;
    Vector2i hovered_cell = {-1, -1};

    int selected_entity = -1;
    Archetype selected_archetype = ARCH_PLAYER;
    bool dragging_level = false;
    int dragged_level = -1;
    Vector2 drag_offset = {0, 0};
};
