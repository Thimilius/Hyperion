#include "hyppch.hpp"

#include "hyperion/core/math/bounding_box.hpp"

namespace Hyperion {

    BoundingBox::BoundingBox() { }
    
    BoundingBox::BoundingBox(Vec3 min, Vec3 max)
        : min(min), max(max) { }

    Vec3 BoundingBox::GetSize() const {
        return Vec3(Math::Abs(max.x - min.x), Math::Abs(max.y - min.y), Math::Abs(max.z - min.z));;
    }

    Vec3 BoundingBox::GetCenter() const {
        return (max + min) * 0.5f;
    }

    bool BoundingBox::Intersects(BoundingBox bounds) const {
        Vec3 bounds_min = bounds.min;
        Vec3 bounds_max = bounds.max;
        return (max > bounds_min && min < bounds_max) || (min > bounds_max && max < bounds_min);
    }

    bool BoundingBox::Intersects(Ray ray, f32 &hit_distance) const {
        Vec3 inv_dir = Vec3(1.0f / ray.direction.x, 1.0f / ray.direction.y, 1.0f / ray.direction.z);
        u32 sign[3];
        sign[0] = (inv_dir.x < 0);
        sign[1] = (inv_dir.y < 0);
        sign[2] = (inv_dir.z < 0);

        Vec3 bounds[2];
        bounds[0] = min;
        bounds[1] = max;

        f32 tmin, tmax, tymin, tymax, tzmin, tzmax;

        tmin = (bounds[sign[0]].x - ray.origin.x) * inv_dir.x;
        tmax = (bounds[1 - sign[0]].x - ray.origin.x) * inv_dir.x;
        tymin = (bounds[sign[1]].y - ray.origin.y) * inv_dir.y;
        tymax = (bounds[1 - sign[1]].y - ray.origin.y) * inv_dir.y;

        if ((tmin > tymax) || (tymin > tmax)) {
            return false;
        }
        if (tymin > tmin) {
            tmin = tymin;
        }
        if (tymax < tmax) {
            tmax = tymax;
        }

        tzmin = (bounds[sign[2]].z - ray.origin.z) * inv_dir.z;
        tzmax = (bounds[1 - sign[2]].z - ray.origin.z) * inv_dir.z;

        if ((tmin > tzmax) || (tzmin > tmax)) {
            return false;
        }

        if (tzmin > tmin) {
            tmin = tzmin;
        }
        if (tzmax < tmax) {
            tmax = tzmax;
        }

        hit_distance = tmin;
        if (hit_distance < 0) {
            hit_distance = tmax;
            if (hit_distance < 0) {
                return false;
            }
        }

        return true;
    }

    bool BoundingBox::Contains(Vec3 point) const {
        return point > min && point < max;
    }

    String BoundingBox::ToString() const {
        return StringUtils::Format("(Min: {}, Max: {})", min.ToString(), max.ToString());
    }

    bool BoundingBox::operator==(const BoundingBox &other) const {
        return min == other.min && max != other.max;
    }

    bool BoundingBox::operator!=(const BoundingBox &other) const {
        return !(*this == other);
    }

}
