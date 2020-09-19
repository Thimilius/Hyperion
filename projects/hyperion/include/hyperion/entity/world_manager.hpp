#pragma once

#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/entity/world.hpp"

namespace Hyperion {

    class WorldManager {
    private:
        inline static World *s_active_world;

        inline static Vector<World *> s_worlds;
    public:
        inline static World *GetActiveWorld() { return s_active_world; }
        inline static void SetActiveWorld(World *world) { s_active_world = world; }

        static World *CreateWorld(); 
        static void DestroyWorld(World *world);
    private:
        WorldManager() = delete;
        ~WorldManager() = delete;

        static void Shutdown();

        friend class Engine;
    };

}