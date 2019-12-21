#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/math/vec3.hpp"
#include "hyperion/core/math/ray.hpp"

namespace Hyperion {

    struct Bounds {
        Vec3 center;
        Vec3 extends;

        Bounds();
        Bounds(Vec3 center, Vec3 size);

        Vec3 GetSize() const;
        Vec3 GetMin() const;
        Vec3 GetMax() const;

        bool Intersects(Bounds bounds);
        bool Intersects(Ray ray);

        void SetMinMax(Vec3 min, Vec3 max);

        String ToString() const;

        bool operator==(const Bounds &other) const;
        bool operator!=(const Bounds &other) const;
    };

}