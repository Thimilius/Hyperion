#pragma once

#include "hyperion/physics/physics_world.hpp"

namespace Hyperion::Physics {

    class PhysicsDriver {
    public:
        virtual ~PhysicsDriver() = default;
        
        virtual PhysicsWorld *CreatePhysicsWorld() = 0;
        virtual void DestroyPhysicsWorld(PhysicsWorld *world) = 0;

        virtual void Init() = 0;
        virtual void Update(f32 delta_time) = 0;
        virtual void Shutdown() = 0;
    };

    class DummyPhysicsDriver : public PhysicsDriver {
    public:
        PhysicsWorld *CreatePhysicsWorld() { return new PhysicsWorld(); };
        void DestroyPhysicsWorld(PhysicsWorld *world) { delete world; }

        void Init() override { HYP_LOG_INFO("Audio", "Initialized dummy physics driver!"); }
        void Update(f32 delta_time) override { }
        void Shutdown() override { }
    };

}