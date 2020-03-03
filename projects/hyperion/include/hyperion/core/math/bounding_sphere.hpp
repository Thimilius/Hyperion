#pragma once

#include "hyperion/core/math/ray.hpp"

namespace Hyperion {

    struct BoundingSphere {
        Vec3 origin;
        f32 radius;

        BoundingSphere();
        BoundingSphere(Vec3 origin, f32 radius);

        bool Intersects(Ray ray, f32 &hit_distance) const;

        String ToString() const;

        bool operator==(const BoundingSphere &other) const;
        bool operator!=(const BoundingSphere &other) const;
    };

}