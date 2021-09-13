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

        static void SetActiveWorld(World *world);
    private:
        static void Initialize();
        static void Update();
        static void Shutdown();
    private:
        inline static World *s_active_world;
        inline static Array<World *> s_worlds;
    private:
        friend class Hyperion::Engine;
    };

}