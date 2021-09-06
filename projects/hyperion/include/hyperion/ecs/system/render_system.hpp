#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/system/system.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Rendering {
    class RenderView;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class CameraSystem : public ISystem {
    public:
        void Run(World *world) override;
    };

}