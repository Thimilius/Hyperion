#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/system/system.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class AssetLoadSystem : public ISystem {
    public:
        void Run(World *world) override;
    };

    class AssetUnloadSystem : public ISystem {
    public:
        void Run(World *world) override;
    };

}
