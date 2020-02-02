#pragma once

#include "hyperion/core/color.hpp"
#include "hyperion/core/math/vec3.hpp"

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
        f32 radius;

        Vec3 position;
        Vec3 direction;

        // TODO: Revisit these properties as they might be to detailed
        struct {
            union {
                f32 attenuation_constant;
                f32 attenuation_linear;
                f32 attenuation_quadratic;
            };
            union {
                f32 spot_inner_radius;
                f32 spot_outer_radius;
            };
        };
    };

}