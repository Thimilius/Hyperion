#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/system/system.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Rendering {
    class RenderFrame;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    class CameraSystem : public ISystem {
    public:
        void Run(World *world) override;
    };

    class SpriteSystem : public ISystem {
    public:
        void Run(World *world) override;
    };

}