//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/application_settings.hpp"
#include "hyperion/physics/physics_driver.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class Engine;
  class World;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Physics {

  class PhysicsEngine final {
  private:
    PhysicsEngine() = delete;
    ~PhysicsEngine() = delete;
  private:
    static void Initialize();
    static void FixedUpdate(float32 delta_time);
    static void Shutdown();

    inline static IPhysicsWorld *CreateWorld(World *world) { return s_physics_driver->CreatePhysicsWorld(world); }
    inline static IPhysicsWorld *CopyWorld(World *world, IPhysicsWorld *physics_world) { return s_physics_driver->CopyPhysicsWorld(world, physics_world); }
    inline static void DestroyWorld(IPhysicsWorld *physics_world) { return s_physics_driver->DestroyPhysicsWorld(physics_world); }
  private:
    inline static IPhysicsDriver *s_physics_driver;
  private:
    friend class Hyperion::Engine;
    friend class Hyperion::World;
  };

}
