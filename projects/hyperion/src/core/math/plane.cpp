#include "hyppch.hpp"

#include "hyperion/core/math/plane.hpp"

namespace Hyperion {

    Plane::Plane()
        : normal(Vec3::Up()), distance(0) { }

    Plane::Plane(Vec3 normal, f32 distance)
        : normal(normal.Normalized()), distance(distance) { }

    Plane::Plane(Vec3 normal, Vec3 point) {
        this->normal = normal.Normalized();
        distance = -this->normal.Dot(point);
    }

    Plane::Plane(Vec3 a, Vec3 b, Vec3 c) {
        normal = Vec3::Cross(b - a, c - a).Normalized();
        distance = -this->normal.Dot(a);
    }

    void Plane::Flip() {
        // TODO: Add negation operator to vectors
        normal = Vec3(-normal.x, -normal.y, -normal.z);
        distance = -distance;
    }

    bool Plane::Intersects(Ray ray, f32 *distance) const {
        float angle = ray.direction.Dot(normal);
        float origin_to_plane = -ray.origin.Dot(normal) - this->distance;
        bool hit;
        if (angle == 0.0f) {
            if (distance) {
                *distance = 0.0f;
            }
            hit = false;
        } else {
            f32 dis = origin_to_plane / angle;
            if (distance) {
                *distance = dis;
            }
            hit = dis > 0.0f;
        }
        return hit;
    }

    String Plane::ToString() const {
        return StringUtils::Format("(Normal: {}, Distance: {})", normal.ToString(), distance);
    }

    bool Plane::operator==(const Plane &other) const {
        return normal == other.normal && distance == other.distance;
    }

    bool Plane::operator!=(const Plane &other) const {
        return !(*this == other);
    }

}