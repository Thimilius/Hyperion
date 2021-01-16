#pragma once

#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/physics/physics_driver.hpp"

namespace Hyperion {
    class Engine;
    class World;
}

namespace Hyperion::Physics {

    class PhysicsEngine final {
    public:
        static bool Raycast(Ray ray, RaycastResult &result, float32 distance = 10000.0f);
    private:
        PhysicsEngine() = delete;
        ~PhysicsEngine() = delete;

        static void Init();
        static void FixedUpdate(float32 delta_time);
        static void Shutdown();

        inline static PhysicsWorld *CreatePhysicsWorld() { return s_physics_driver->CreatePhysicsWorld(); }
        inline static void DestroyPhysicsWorld(PhysicsWorld *world) { return s_physics_driver->DestroyPhysicsWorld(world); }
    private:
        inline static IPhysicsDriver *s_physics_driver;
    private:
        friend class Hyperion::Engine;
        friend class Hyperion::World;
    };

}