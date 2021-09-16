#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/system/system.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class LocalToWorldSystem : public ISystem {
    public:
        void Run(World *world) override;
    };

}