#pragma once

#include "hyperion/physics/physics_backend.hpp"
#include "hyperion/physics/physics_driver.hpp"

namespace Hyperion {
    class Engine;
}

namespace Hyperion::Physics {

    class PhysicsEngine {
    private:
        inline static PhysicsBackend s_physics_backend;

        inline static Scope<PhysicsDriver> s_physics_driver;
    public:
        inline static PhysicsBackend GetBackend() { return s_physics_backend; }

        inline static bool Raycast(Ray ray, RaycastResult &result, f32 distance = 10000.0f) { s_physics_driver->Raycast(ray, result, distance); }

        inline static PhysicsWorld *CreatePhysicsWorld() { return s_physics_driver->CreatePhysicsWorld(); }
        inline static void DestroyPhysicsWorld(PhysicsWorld *world) { return s_physics_driver->DestroyPhysicsWorld(world); }
    private:
        PhysicsEngine() = delete;
        ~PhysicsEngine() = delete;

        static void Init(PhysicsBackend backend);
        static void Update(f32 delta_time) { s_physics_driver->Update(delta_time); }
        static void Shutdown() { s_physics_driver->Shutdown(); }

        friend class Hyperion::Engine;
    };

}