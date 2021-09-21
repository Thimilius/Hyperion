#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    struct WorldEnvironment {
        struct AmbientLight {
            float32 intensity = 0.2f;
            Color color = Color::White();
        } ambient_light;
    };

}