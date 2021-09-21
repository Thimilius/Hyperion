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

    class NullPhysicsDriver : public IPhysicsDriver {
    public:
        virtual PhysicsBackend GetBackend() const { return PhysicsBackend::None; }

        virtual IPhysicsWorld *CreatePhysicsWorld(World *world) { return new NullPhysicsWorld(); }
        virtual void DestroyPhysicsWorld(IPhysicsWorld *world) { delete world; }

        virtual void Initialize() { HYP_LOG_INFO("Physics", "Initialized Null audio driver!"); }
        virtual void FixedUpdate(float32 delta_time) { }
        virtual void Shutdown() { }
    };

}