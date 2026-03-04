#pragma once
#include "raylib.h"

namespace CameraController{

    struct CameraController
    {
        Camera2D cam;

        float zoom = 1.0f;
        float zoom_min = 0.2f;
        float zoom_max = 4.0f;

        bool dragging = false;
        Vector2 drag_start_mouse{};
        Vector2 drag_start_target{};

        Vector2 default_target{};
        float default_zoom = 1.0f;
    };



    void CameraInit(CameraController& c, int screenW, int screenH);
    void CameraUpdateZoom(CameraController& c);
    void CameraUpdatePan(CameraController& c);
    void CameraReset(CameraController& c);
    void CameraUpdate(CameraController& c);
    void SetTarget(CameraController& c, Vector2 target);
}
