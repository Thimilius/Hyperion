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
        normal = -normal;
        distance = -distance;
    }

    Vec3 Plane::GetClosestPoint(Vec3 point) const {
        f32 d = normal.Dot(point) + distance;
        return point - normal * d;
    }

    f32 Plane::GetDistanceToPoint(Vec3 point) const {
        return normal.Dot(point) + distance;
    }

    bool Plane::Intersects(Ray ray, f32 &hit_distance) const {
        f32 angle = ray.direction.Dot(normal);
        f32 origin_to_plane = -ray.origin.Dot(normal) - this->distance;
        bool hit;
        if (angle == 0.0f) {
            hit_distance = 0.0f;
            hit = false;
        } else {
            f32 dis = origin_to_plane / angle;
            hit_distance = dis;
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