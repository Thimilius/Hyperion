#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/ray.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    struct BoundingBox {
        Vector3 min;
        Vector3 max;

        BoundingBox();
        BoundingBox(Vector3 min, Vector3 max);

        Vector3 GetSize() const;
        Vector3 GetCenter() const;

        bool8 Intersects(BoundingBox other) const;
        bool8 Intersects(Ray ray, float32 &hit_distance) const;
        bool8 Contains(Vector3 point) const;

        String ToString() const;

        bool8 operator==(const BoundingBox &other) const;
        bool8 operator!=(const BoundingBox &other) const;
    };

}