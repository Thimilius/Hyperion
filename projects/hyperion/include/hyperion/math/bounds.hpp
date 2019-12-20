#pragma once

#include "hyperion/common.hpp"
#include "hyperion/math/vec3.hpp"

namespace Hyperion {

    struct Bounds {
        Vec3 center;
        Vec3 extends;

        Bounds();
        Bounds(Vec3 center, Vec3 extends);

        Vec3 GetSize() const;
        Vec3 GetMin() const;
        Vec3 GetMax() const;

        void SetMinMax(Vec3 min, Vec3 max);

        String ToString() const;
    };

}