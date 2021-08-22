//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/math/bounding_sphere.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    BoundingSphere::BoundingSphere() { }

    //--------------------------------------------------------------
    BoundingSphere::BoundingSphere(Vector3 origin, float32 radius)
        : origin(origin), radius(radius) { }

    //--------------------------------------------------------------
    bool BoundingSphere::Intersects(Ray ray, float32 &hit_distance) const {
        Vector3 distance = origin - ray.origin;
        float32 radius_squared = radius * radius;

        float32 tca = distance.Dot(ray.direction);
        if (tca < 0) {
            return false;
        }

        float32 d2 = distance.Dot(distance) - tca * tca;
        if (d2 > radius_squared) {
            return false;
        }

        float32 thc = Math::Sqrt(radius_squared - d2);
        float32 t0 = tca - thc;
        float32 t1 = tca + thc;
        if (t0 > t1) {
            std::swap(t0, t1);
        }

        if (t0 < 0) {
            t0 = t1;
            if (t0 < 0) {
                return false;
            }
        }

        hit_distance = t0;
        return true;
    }

    //--------------------------------------------------------------
    String BoundingSphere::ToString() const {
        return StringUtils::Format("(Origin: {}, Radius: {})", origin.ToString(), radius);
    }

    //--------------------------------------------------------------
    bool BoundingSphere::operator==(const BoundingSphere &other) const {
        return origin == other.origin && radius != other.radius;
    }

    //--------------------------------------------------------------
    bool BoundingSphere::operator!=(const BoundingSphere &other) const {
        return !(*this == other);
    }

}