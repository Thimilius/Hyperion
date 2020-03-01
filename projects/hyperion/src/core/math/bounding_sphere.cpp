#include "hyppch.hpp"

#include "hyperion/core/math/bounding_sphere.hpp"

namespace Hyperion {

    BoundingSphere::BoundingSphere() { }

    BoundingSphere::BoundingSphere(Vec3 origin, f32 radius)
        : origin(origin), radius(radius) { }

    bool BoundingSphere::Intersects(Ray ray) const {
        Vec3 distance = origin - ray.origin;
        f32 radius_squared = radius * radius;

        f32 tca = distance.Dot(ray.direction);
        if (tca < 0) {
            return false;
        }

        f32 d2 = distance.Dot(distance) - tca * tca;
        if (d2 > radius_squared) {
            return false;
        }

        f32 thc = Math::Sqrt(radius_squared - d2);
        f32 t0 = tca - thc;
        f32 t1 = tca + thc;
        if (t0 > t1) {
            std::swap(t0, t1);
        }

        if (t0 < 0) {
            t0 = t1;
            if (t0 < 0) {
                return false;
            }
        }

        f32 hit = t0;
        return true;
    }

    String BoundingSphere::ToString() const {
        return StringUtils::Format("(Origin: {}, Radius: {})", origin.ToString(), radius);
    }

    bool BoundingSphere::operator==(const BoundingSphere &other) const {
        return origin == other.origin && radius != other.radius;
    }

    bool BoundingSphere::operator!=(const BoundingSphere &other) const {
        return !(*this == other);
    }

}