#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/application_settings.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Engine;
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
    private:
        friend class Hyperion::Engine;
    };

}