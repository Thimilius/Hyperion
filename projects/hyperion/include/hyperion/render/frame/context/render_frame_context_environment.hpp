#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct RenderFrameContextEnvironment {
        struct AmbientLight {
            float32 intensity;
            Color color;
        } ambient_light;
    };

}