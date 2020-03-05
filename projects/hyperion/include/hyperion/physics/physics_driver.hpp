#pragma once

#include "hyperion/common.hpp"

namespace Hyperion::Physics {

    class PhysicsDriver {
    public:
        virtual ~PhysicsDriver() = default;

        virtual void Init() = 0;
        virtual void Update(f32 delta_time) = 0;
        virtual void Shutdown() = 0;
    };

    class DummyPhysicsDriver : public PhysicsDriver {
    public:
        void Init() override { HYP_LOG_INFO("Audio", "Initialized dummy physics driver!"); }
        void Update(f32 delta_time) override { }
        void Shutdown() override { }
    };

}