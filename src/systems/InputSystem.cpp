#include "InputSystem.hpp"

namespace InputSystem
{
    void Update(World& w)
    {
        if (IsKeyPressed(KEY_G))
        {
            StartMoving(w);
        }
    }
}
