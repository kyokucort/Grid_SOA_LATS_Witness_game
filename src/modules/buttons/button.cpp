#include "button.hpp"

Button CreateButton(Vector2 pos, Vector2 size, const char *text)
{
    Button button;
    button.size = size;
    button.position = pos;
    button.text = text;
    button.rect.x = pos.x;
    button.rect.y = pos.y;
    button.rect.width = size.x;
    button.rect.height = size.y;
    button.base_color = GRAY;
    button.highlight_color = WHITE;
    button.is_under_cursor = false;

    return button;
}

void DrawButton(Button button)
{
    if (button.is_under_cursor){
        DrawRectangle(button.position.x, button.position.y, button.size.x, button.size.y, button.highlight_color);
    }else{
        DrawRectangle(button.position.x, button.position.y, button.size.x, button.size.y, button.base_color);
    }
    DrawText(button.text, button.position.x, button.position.y, 40, BLACK);
}

void IsUnderCursor(Button *button)
{
    button->is_under_cursor = CheckCollisionPointRec(GetMousePosition(), button->rect);
}

bool CheckForClick(Button button)
{
    if (button.is_under_cursor == false) return false;
    if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)){
        return true;
    }
    return false;

}
