#pragma once

#include "hyperion/common.hpp"

namespace Hyperion {

    typedef void (*EngineLoopUpdateFunction)();

    struct EngineLoopSubSystem {
        String name;
        EngineLoopUpdateFunction update_function = nullptr;
        Vector<EngineLoopSubSystem> sub_systems;
    };

    struct EngineLoopSystem {
        EngineLoopSubSystem initilization;
        EngineLoopSubSystem fixed_update;
        EngineLoopSubSystem tick;
        EngineLoopSubSystem pre_update;
        EngineLoopSubSystem update;
        EngineLoopSubSystem late_update;

        static EngineLoopSystem Default();
    };

}