//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/math/bounding_box.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    BoundingBox::BoundingBox() { }
    
    //--------------------------------------------------------------
    BoundingBox::BoundingBox(Vector3 min, Vector3 max)
        : min(min), max(max) { }

    //--------------------------------------------------------------
    Vector3 BoundingBox::GetSize() const {
        return Vector3(Math::Abs(max.x - min.x), Math::Abs(max.y - min.y), Math::Abs(max.z - min.z));;
    }

    //--------------------------------------------------------------
    Vector3 BoundingBox::GetCenter() const {
        return (max + min) * 0.5f;
    }

    //--------------------------------------------------------------
    bool BoundingBox::Intersects(BoundingBox other) const {
        Vector3 other_min = other.min;
        Vector3 other_max = other.max;
        return (max > other_min && min < other_max) || (min > other_max && max < other_min);
    }

    //--------------------------------------------------------------
    bool BoundingBox::Intersects(Ray ray, float32 &hit_distance) const {
        Vector3 inv_dir = Vector3(1.0f / ray.direction.x, 1.0f / ray.direction.y, 1.0f / ray.direction.z);
        uint32 sign[3];
        sign[0] = (inv_dir.x < 0);
        sign[1] = (inv_dir.y < 0);
        sign[2] = (inv_dir.z < 0);

        Vector3 bounds[2];
        bounds[0] = min;
        bounds[1] = max;

        float32 tmin, tmax, tymin, tymax, tzmin, tzmax;

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

    //--------------------------------------------------------------
    bool BoundingBox::Contains(Vector3 point) const {
        return point > min && point < max;
    }

    //--------------------------------------------------------------
    String BoundingBox::ToString() const {
        return StringUtils::Format("(Min: {}, Max: {})", min.ToString(), max.ToString());
    }

    //--------------------------------------------------------------
    bool BoundingBox::operator==(const BoundingBox &other) const {
        return min == other.min && max != other.max;
    }

    //--------------------------------------------------------------
    bool BoundingBox::operator!=(const BoundingBox &other) const {
        return !(*this == other);
    }

}
