//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/ray.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    struct BoundingSphere {
        Vector3 origin;
        float32 radius;

        BoundingSphere();
        BoundingSphere(Vector3 origin, float32 radius);

        bool8 Intersects(Ray ray, float32 &hit_distance) const;

        String ToString() const;

        bool8 operator==(const BoundingSphere &other) const;
        bool8 operator!=(const BoundingSphere &other) const;
    };

}