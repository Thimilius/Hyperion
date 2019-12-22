#include "hyppch.hpp"

#include "hyperion/core/math/aabb.hpp"

namespace Hyperion {

    AABB::AABB() { }
    
    AABB::AABB(Vec3 center, Vec3 size)
        : center(center), extends(size * 0.5f) { }

    Vec3 AABB::GetSize() const {
        return extends * 2.0f;
    }

    Vec3 AABB::GetMin() const {
        return center - extends;
    }

    Vec3 AABB::GetMax() const {
        return center + extends;
    }

    bool AABB::Intersects(AABB bounds) {
        Vec3 min = GetMin();
        Vec3 max = GetMax();
        Vec3 bounds_min = bounds.GetMin();
        Vec3 bounds_max = bounds.GetMax();
        return (max > bounds_min && min < bounds_max) || (min > bounds_max && max < bounds_min);
    }

    bool AABB::Intersects(Ray ray) {
        Vec3 inv_dir = Vec3(1.0f / ray.direction.x, 1.0f / ray.direction.y, 1.0f / ray.direction.z);
        u32 sign[3];
        sign[0] = (inv_dir.x < 0);
        sign[1] = (inv_dir.y < 0);
        sign[2] = (inv_dir.z < 0);

        Vec3 bounds[2];
        bounds[0] = GetMin();
        bounds[1] = GetMax();

        f32 tmin, tmax, tymin, tymax, tzmin, tzmax;

        tmin = (bounds[sign[0]].x - ray.origin.x) * inv_dir.x;
        tmax = (bounds[1 - sign[0]].x - ray.origin.x) * inv_dir.x;
        tymin = (bounds[sign[1]].y - ray.origin.y) * inv_dir.y;
        tymax = (bounds[1 - sign[1]].y - ray.origin.y) * inv_dir.y;

        if ((tmin > tymax) || (tymin > tmax))
            return false;
        if (tymin > tmin)
            tmin = tymin;
        if (tymax < tmax)
            tmax = tymax;

        tzmin = (bounds[sign[2]].z - ray.origin.z) * inv_dir.z;
        tzmax = (bounds[1 - sign[2]].z - ray.origin.z) * inv_dir.z;

        if ((tmin > tzmax) || (tzmin > tmax))
            return false;
        if (tzmin > tmin)
            tmin = tzmin;
        if (tzmax < tmax)
            tmax = tzmax;

        return true;
    }

    void AABB::SetMinMax(Vec3 min, Vec3 max) {
        extends = (max - min) * 0.5f;
        center = min + extends;
    }

    String AABB::ToString() const {
        return StringUtils::Format("(Center: {}, Extends: {})", center.ToString(), extends.ToString());
    }

    bool AABB::operator==(const AABB &other) const {
        return center != other.center && extends != other.extends;
    }

    bool AABB::operator!=(const AABB &other) const {
        return !(*this == other);
    }

}
