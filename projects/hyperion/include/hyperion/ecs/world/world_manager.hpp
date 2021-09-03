#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/world/world.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Engine;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class WorldManager final {
    private:
        WorldManager() = delete;
        ~WorldManager() = delete;
    public:
        static World *CreateWorld();
        static void DestroyWorld(World *world);
    private:
        static void Initialize();
        static void Shutdown();
    private:
        inline static List<World *> s_worlds;
    private:
        friend class Hyperion::Engine;
    };

}