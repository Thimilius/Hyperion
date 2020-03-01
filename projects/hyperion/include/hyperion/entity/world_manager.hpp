#pragma once

#include "hyperion/entity/world.hpp"

namespace Hyperion {

    class WorldManager {
    private:
        inline static World *s_active_world;

        inline static Vector<World *> s_worlds;
    public:
        inline static World *GetActiveWorld() { return s_active_world; }
        inline static void SetActiveWorld(World *world) { 
            HYP_ASSERT_MESSAGE(world, "Active world can not be null!");
            s_active_world = world;
        }

        static World *CreateWorld(); 
        static void DestroyWorld(World *world);
    private:
        WorldManager() = delete;
        ~WorldManager() = delete;
    };

}