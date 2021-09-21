#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/render/types/render_types_light.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct RenderFrameContextLight {
        LightType type;

        Color color;
        float32 intensity;

        Vector3 direction;
        Vector3 position;

        float32 range = 10.0f;

        float32 inner_spot_radius = 1.0f;
        float32 outer_spot_radius = 1.0f;
    };

}