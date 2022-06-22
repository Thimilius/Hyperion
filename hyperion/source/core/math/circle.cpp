//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/math/circle.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  Circle::Circle() : center(Vector3()), orientation(Vector3::Up()), radius(0.5f) { }

  //--------------------------------------------------------------
  Circle::Circle(Vector3 center, Vector3 orientation, float32 radius) : center(center), orientation(orientation), radius(radius) { }
  
  //--------------------------------------------------------------
  float32 Circle::GetClosestPointToRay(Ray ray, Vector3 &point) {
    Plane plane = Plane(orientation, center);

    float32 distance = 0.0f;
    if (plane.Intersects(ray, distance)) {
      Vector3 on_plane = ray.GetPoint(distance);
      point = center + radius * (on_plane - center).Normalized();
      return (on_plane - point).Magnitude();
    } else {
      Vector3 a = ray.origin - center;
      Vector3 b = orientation;
      Vector3 rejection = a - (a.Dot(b) / b.Dot(b)) * b;

      point = radius * rejection.Normalized() - center;

      float32 distance_to_point = Vector3::Cross(ray.direction, point - ray.origin).Magnitude();
      return distance_to_point;
    }
  }

  //--------------------------------------------------------------
  float32 Circle::GetAngleBetweenPoints(Vector3 a, Vector3 b) {
    float32 dot = a.Dot(b);
    dot = Math::Clamp(dot, -1.0f, 1.0f);

    if (dot >= 0.9999999f) {
      dot = 1.0f;
    }
          
    float32 angle = Math::ACos(dot);
    float32 degree = Math::RadToDeg(angle);

    float32 circle_dot = Vector3::Cross(b, a).Dot(orientation);
    float32 mag = (Vector3::Cross(b, a) * (orientation)).Magnitude();

    float32 sign = 1.0f;
    if (mag != 0.0f) {
      sign = circle_dot / mag;  
    }

    degree *= sign;

    return degree;
  }

}
  