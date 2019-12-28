#pragma once

#include "hyperion/core/math/vec3.hpp"

namespace Hyperion {

    struct Ray {
        Vec3 origin;
        Vec3 direction;

        Ray(Vec3 origin, Vec3 direction);

        Vec3 GetPoint(f32 t) const;

        String ToString() const;
    };

}