#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "raylib.h"

typedef struct Button{
    Vector2 size;
    Vector2 position;
    Rectangle rect;
    const char *text;
    Color base_color;
    Color highlight_color;
    bool is_under_cursor;
}Button;

Button CreateButton(Vector2 pos, Vector2 size, const char *text);
void DrawButton(Button button);
void IsUnderCursor(Button *button);
bool CheckForClick(Button button);

#endif
