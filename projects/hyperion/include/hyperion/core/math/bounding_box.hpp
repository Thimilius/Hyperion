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

        bool Intersects(BoundingBox other) const;
        bool Intersects(Ray ray, float32 &hit_distance) const;
        bool Contains(Vector3 point) const;

        String ToString() const;

        bool operator==(const BoundingBox &other) const;
        bool operator!=(const BoundingBox &other) const;
    };

}