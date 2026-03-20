#include "LogicSystem.hpp"

namespace LogicSystem
{

    bool CheckRule(uint32_t state, LogicRule& rule)
    {
        printf("BBAAAAASEEE\n");
        if ((state & rule.require) != rule.require)
            return false;

        printf("COOOOL\n");
        if (state & rule.forbid)
            return false;

        printf("KKKKKK\n");
        return true;
    }


    void LogicSystem(World& w, uint32_t state)
    {
        for (int e = 0; e < MAX_ENTITIES; e++)
        {
            if (!w.logic.has[e])
                continue;

            if (!w.hover.hovered[e])
                continue;

            if (CheckRule(state, w.logic.rule[e]))
            {
                w.entity.alive[e] = false;
                //ActivateEntity(w, e);
            }
        }
    }
}
