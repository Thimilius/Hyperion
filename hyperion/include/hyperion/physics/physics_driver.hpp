//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/physics/physics_backend.hpp"
#include "hyperion/physics/physics_world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Physics {

  class IPhysicsDriver {
  public:
    virtual ~IPhysicsDriver() = default;
  public:
    virtual PhysicsBackend GetBackend() const = 0;

    virtual void Initialize() = 0;
    virtual void FixedUpdate(float32 delta_time) = 0;
    virtual void Shutdown() = 0;

    virtual IPhysicsWorld *CreateWorld(World *world) = 0;
    virtual IPhysicsWorld *CopyWorld(World *world, IPhysicsWorld *physics_world) = 0;
    virtual void DestroyWorld(IPhysicsWorld *physics_world) = 0;
  };

  class NullPhysicsDriver final : public IPhysicsDriver {
  public:
    PhysicsBackend GetBackend() const override { return PhysicsBackend::None; }

    void Initialize() override { HYP_LOG_INFO("Physics", "Initialized Null physics driver!"); }
    void FixedUpdate(float32 delta_time) override { }
    void Shutdown() override { }

    IPhysicsWorld *CreateWorld(World *world) override { return new NullPhysicsWorld(); }
    IPhysicsWorld *CopyWorld(World *world, IPhysicsWorld *physics_world) override { return new NullPhysicsWorld(); }
    void DestroyWorld(IPhysicsWorld *physics_world) override { delete physics_world; }
  };

}
