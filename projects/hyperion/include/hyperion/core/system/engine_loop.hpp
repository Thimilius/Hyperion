#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    typedef void (*EngineLoopUpdateFunction)();

    struct EngineLoopSubSystem {
        String name;
        EngineLoopUpdateFunction update_function = nullptr;
        List<EngineLoopSubSystem> sub_systems;
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