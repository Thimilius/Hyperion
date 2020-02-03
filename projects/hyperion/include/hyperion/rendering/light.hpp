#pragma once

#include "hyperion/core/color.hpp"

namespace Hyperion::Rendering {

    enum class LightType {
        Directional,
        Point,
        Spot
    };

    struct Light {
        LightType type;

        Color color;
        f32 intensity;

        f32 range;

        f32 spot_inner_radius;
        f32 spot_outer_radius;
    };

}