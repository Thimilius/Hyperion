#pragma once

#include "hyperion/physics/physics_backend.hpp"
#include "hyperion/physics/physics_world.hpp"

namespace Hyperion::Physics {

    class IPhysicsDriver {
    public:
        virtual ~IPhysicsDriver() = default;
        
        virtual PhysicsBackend GetBackend() const = 0;

        virtual PhysicsWorld *CreatePhysicsWorld() = 0;
        virtual void DestroyPhysicsWorld(PhysicsWorld *world) = 0;

        virtual void Init() = 0;
        virtual void FixedUpdate(f32 delta_time) = 0;
        virtual void Shutdown() = 0;
    };

    class DummyPhysicsDriver : public IPhysicsDriver {
    public:
        PhysicsBackend GetBackend() const override { return PhysicsBackend::None; }

        PhysicsWorld *CreatePhysicsWorld() { return new PhysicsWorld(); };
        void DestroyPhysicsWorld(PhysicsWorld *world) { delete world; }

        void Init() override { HYP_LOG_INFO("Physics", "Initialized dummy physics driver!"); }
        void FixedUpdate(f32 delta_time) override { }
        void Shutdown() override { }
    };

}