#pragma once

#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/entity/world.hpp"

namespace Hyperion {
    class Component;
    class Engine;
}

namespace Hyperion {

    class WorldManager final {
    public:
        inline static World *GetActiveWorld() { return s_active_world; }
        inline static void SetActiveWorld(World *world) { s_active_world = world; }

        static World *CreateWorld(); 
        static void DestroyWorld(World *world);
    private:
        WorldManager() = delete;
        ~WorldManager() = delete;

        static void Shutdown();

        static void Update(float32 delta_time);

        static void RegisterComponentForUpdate(Component *component);
        static void UnregisterComponentForUpdate(Component *component);
    private:
        inline static World *s_active_world;

        inline static Vector<World *> s_worlds;
        inline static Map<World *, Set<Component *>> s_components_to_update;
    private:
        friend class Hyperion::Component;
        friend class Hyperion::Engine;
    };

}