#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/math/vec3.hpp"
#include "hyperion/core/math/ray.hpp"

namespace Hyperion {

    struct BoundingBox {
        Vec3 min;
        Vec3 max;

        BoundingBox();
        BoundingBox(Vec3 min, Vec3 max);

        Vec3 GetSize() const;
        Vec3 GetCenter() const;

        bool Intersects(BoundingBox bounds) const;
        bool Intersects(Ray ray) const;
        bool Contains(Vec3 point) const;

        String ToString() const;

        bool operator==(const BoundingBox &other) const;
        bool operator!=(const BoundingBox &other) const;
    };

}