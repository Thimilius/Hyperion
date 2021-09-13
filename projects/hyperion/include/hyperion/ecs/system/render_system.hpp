#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/system/system.hpp"

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

    class RenderMeshSystem : public ISystem {
    public:
        void Run(World *world) override;
    };

    class RenderAssetUnloadSystem : public ISystem {
    public:
        void Run(World *world) override;
    };

}