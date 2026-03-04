#include "CameraController.hpp"
#include "raymath.h"

namespace CameraController{
    void CameraInit(CameraController& c, int screenW, int screenH)
    {
        c.cam.target = {0,0};
        c.cam.offset = {(float)screenW/2, (float)screenH/2};
        c.cam.rotation = 0;
        c.cam.zoom = 1;

        c.default_target = c.cam.target;
        c.default_zoom = c.cam.zoom;
    }


    void CameraUpdateZoom(CameraController& c)
    {
        float wheel = GetMouseWheelMove();
        if(wheel == 0) return;

        Vector2 mouseWorldBefore = GetScreenToWorld2D(GetMousePosition(), c.cam);

        c.zoom += wheel * 0.1f;
        c.zoom = Clamp(c.zoom, c.zoom_min, c.zoom_max);
        c.cam.zoom = c.zoom;

        Vector2 mouseWorldAfter = GetScreenToWorld2D(GetMousePosition(), c.cam);
        Vector2 delta = Vector2Subtract(mouseWorldBefore, mouseWorldAfter);

        c.cam.target = Vector2Add(c.cam.target, delta);
    }

    void CameraUpdatePan(CameraController& c)
    {
        if(IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE))
        {
            c.dragging = true;
            c.drag_start_mouse = GetMousePosition();
            c.drag_start_target = c.cam.target;
        }

        if(IsMouseButtonReleased(MOUSE_BUTTON_MIDDLE))
            c.dragging = false;

        if(c.dragging)
        {
            Vector2 delta = Vector2Subtract(c.drag_start_mouse, GetMousePosition());
            delta = Vector2Scale(delta, 1.0f / c.cam.zoom);
            c.cam.target = Vector2Add(c.drag_start_target, delta);
        }
    }

    void CameraReset(CameraController& c)
    {
        c.cam.target = c.default_target;
        c.zoom = c.default_zoom;
        c.cam.zoom = c.zoom;
    }

    void CameraUpdate(CameraController& c)
    {
        CameraUpdateZoom(c);
        CameraUpdatePan(c);

        if(IsKeyPressed(KEY_R))
            CameraReset(c);
    }

    void SetTarget(CameraController& c, Vector2 target)
    {
        c.cam.target = target;
    }
}
