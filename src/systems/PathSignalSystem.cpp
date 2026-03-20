#include "PathSignalSystem.hpp"

#include "systems/CursorSystem.hpp"

namespace PathSignalSystem
{
    uint32_t ComputePathSignal(World& w, std::vector<Vector2i>& path)
    {
        if (path.empty())
            return SIGNAL_NONE;

        uint32_t state = 0;

        int start = CursorSystem::HasInteractible(w, path.front());

        if (start >= 0 && w.signal.has[start])
            state |= w.signal.data[start].signal;

        for (auto& cell : path)
        {
            int e = CursorSystem::HasInteractible(w, cell);

            if (e < 0)
                continue;

            if (w.modifier.has[e])
            {
                auto& m = w.modifier.data[e];

                state |= m.add;
                state &= ~m.remove;
            }
        }

        return state;
    }
}
