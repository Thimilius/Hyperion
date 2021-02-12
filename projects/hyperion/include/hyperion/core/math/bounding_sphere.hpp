#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/ray.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    struct BoundingSphere {
        Vec3 origin;
        float32 radius;

        BoundingSphere();
        BoundingSphere(Vec3 origin, float32 radius);

        bool Intersects(Ray ray, float32 &hit_distance) const;

        String ToString() const;

        bool operator==(const BoundingSphere &other) const;
        bool operator!=(const BoundingSphere &other) const;
    };

}