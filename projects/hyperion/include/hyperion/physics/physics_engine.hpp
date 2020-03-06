#pragma once

#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/physics/physics_driver.hpp"

namespace Hyperion {
    class Engine;
    class World;
}

namespace Hyperion::Physics {

    class PhysicsEngine {
    private:
        inline static PhysicsBackend s_physics_backend;

        inline static Scope<PhysicsDriver> s_physics_driver;
    public:
        inline static PhysicsBackend GetBackend() { return s_physics_backend; }

        inline static bool Raycast(Ray ray, RaycastResult &result, f32 distance = 10000.0f) { return s_physics_driver->Raycast(ray, result, distance); }
    private:
        PhysicsEngine() = delete;
        ~PhysicsEngine() = delete;

        static void Init(const PhysicsSettings &settings);
        static void Update(f32 delta_time) { s_physics_driver->Update(delta_time); }
        static void Shutdown() { s_physics_driver->Shutdown(); }

        inline static PhysicsWorld *CreatePhysicsWorld() { return s_physics_driver->CreatePhysicsWorld(); }
        inline static void DestroyPhysicsWorld(PhysicsWorld *world) { return s_physics_driver->DestroyPhysicsWorld(world); }

        friend class Hyperion::Engine;
        friend class Hyperion::World;
    };

}