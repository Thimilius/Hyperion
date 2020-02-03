#pragma once

#include "hyperion/core/color.hpp"

namespace Hyperion::Rendering {

    enum class LightType {
        Directional,
        Point,
        Spot
    };

    struct Light {
        LightType type = LightType::Point;

        Color color = Color::White();
        f32 intensity = 1.0f;

        f32 range = 50.0f;

        f32 inner_spot_radius = 1.0f;
        f32 outer_spot_radius = 1.0f;
    };

}