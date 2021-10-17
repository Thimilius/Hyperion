#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/system/system.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::UI {

    class UISystem : public ISystem {
    public:
        void Run(World *world) override;
    };

}