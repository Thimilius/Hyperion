//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/math/plane.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Plane::Plane()
        : normal(Vector3::Up()), distance(0) { }

    //--------------------------------------------------------------
    Plane::Plane(Vector3 normal, float32 distance)
        : normal(normal.Normalized()), distance(distance) { }

    //--------------------------------------------------------------
    Plane::Plane(Vector3 normal, Vector3 point) {
        this->normal = normal.Normalized();
        distance = -this->normal.Dot(point);
    }

    //--------------------------------------------------------------
    Plane::Plane(Vector3 a, Vector3 b, Vector3 c) {
        normal = Vector3::Cross(b - a, c - a).Normalized();
        distance = -this->normal.Dot(a);
    }

    //--------------------------------------------------------------
    void Plane::Flip() {
        normal = -normal;
        distance = -distance;
    }

    //--------------------------------------------------------------
    Vector3 Plane::GetClosestPoint(Vector3 point) const {
        float32 d = normal.Dot(point) + distance;
        return point - normal * d;
    }

    //--------------------------------------------------------------
    float32 Plane::GetDistanceToPoint(Vector3 point) const {
        return normal.Dot(point) + distance;
    }

    //--------------------------------------------------------------
    bool8 Plane::Intersects(Ray ray, float32 &hit_distance) const {
        float32 angle = ray.direction.Dot(normal);
        float32 origin_to_plane = -ray.origin.Dot(normal) - this->distance;
        bool8 hit;
        if (angle == 0.0f) {
            hit_distance = 0.0f;
            hit = false;
        } else {
            float32 dis = origin_to_plane / angle;
            hit_distance = dis;
            hit = dis > 0.0f;
        }
        return hit;
    }

    //--------------------------------------------------------------
    String Plane::ToString() const {
        return StringUtils::Format("(Normal: {}, Distance: {})", normal.ToString(), distance);
    }

    //--------------------------------------------------------------
    bool8 Plane::operator==(const Plane &other) const {
        return normal == other.normal && distance == other.distance;
    }

    //--------------------------------------------------------------
    bool8 Plane::operator!=(const Plane &other) const {
        return !(*this == other);
    }

}