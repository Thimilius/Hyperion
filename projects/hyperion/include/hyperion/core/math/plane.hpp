#pragma once

#include "hyperion/core/math/vec3.hpp"
#include "hyperion/core/math/ray.hpp"

namespace Hyperion {

    struct Plane {
        Vec3 normal;
        f32 distance;

        Plane();
        Plane(Vec3 normal, f32 distance);
        Plane(Vec3 normal, Vec3 point);
        Plane(Vec3 a, Vec3 b, Vec3 c);

        void Flip();

        Vec3 GetClosestPoint(Vec3 point) const;
        f32 GetDistanceToPoint(Vec3 point) const;

        bool Intersects(Ray ray, f32 &hit_distance) const;

        String ToString() const;

        bool operator==(const Plane &other) const;
        bool operator!=(const Plane &other) const;
    };

}