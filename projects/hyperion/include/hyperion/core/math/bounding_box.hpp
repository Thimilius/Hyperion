#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/ray.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    struct BoundingBox {
        Vec3 min;
        Vec3 max;

        BoundingBox();
        BoundingBox(Vec3 min, Vec3 max);

        Vec3 GetSize() const;
        Vec3 GetCenter() const;

        bool Intersects(BoundingBox bounds) const;
        bool Intersects(Ray ray, float32 &hit_distance) const;
        bool Contains(Vec3 point) const;

        String ToString() const;

        bool operator==(const BoundingBox &other) const;
        bool operator!=(const BoundingBox &other) const;
    };

}