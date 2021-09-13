#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    using EngineLoopUpdateFunction = void(*)();

    struct EngineLoopSubSystem {
        String name;
        EngineLoopUpdateFunction update_function = nullptr;
        Array<EngineLoopSubSystem> sub_systems;
    };

    struct EngineLoopSystem {
        EngineLoopSubSystem initilization;
        EngineLoopSubSystem fixed_update;
        EngineLoopSubSystem tick;
        EngineLoopSubSystem pre_update;
        EngineLoopSubSystem update;
        EngineLoopSubSystem late_update;
    };

}