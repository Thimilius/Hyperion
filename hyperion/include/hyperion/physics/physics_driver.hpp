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

    virtual IPhysicsWorld *CreatePhysicsWorld(World *world) = 0;
    virtual void DestroyPhysicsWorld(IPhysicsWorld *world) = 0;

    virtual void Initialize() = 0;
    virtual void FixedUpdate(float32 delta_time) = 0;
    virtual void Shutdown() = 0;
  };

  class NullPhysicsDriver final : public IPhysicsDriver {
  public:
    PhysicsBackend GetBackend() const override { return PhysicsBackend::None; }

    IPhysicsWorld *CreatePhysicsWorld(World *world) override { return new NullPhysicsWorld(); }
    void DestroyPhysicsWorld(IPhysicsWorld *world) override { delete world; }

    void Initialize() override { HYP_LOG_INFO("Physics", "Initialized Null physics driver!"); }

    void FixedUpdate(float32 delta_time) override { }
    void Shutdown() override { }
  };

}
