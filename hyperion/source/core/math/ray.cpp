//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/math/ray.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  Ray::Ray() { }

  //--------------------------------------------------------------
  Ray::Ray(Vector3 origin, Vector3 direction) :
    origin(origin), direction(direction.Normalized()) { }

  //--------------------------------------------------------------
  Vector3 Ray::GetPoint(float32 t) const {
    return origin + direction * t;
  }

  //--------------------------------------------------------------
  Vector3 Ray::GetClosestPointToRay(Ray ray) const {
    Vector3 dp = ray.origin - origin;
    float32 v12 = direction.Dot(direction);
    float32 v22 = ray.direction.Dot(ray.direction);
    float32 v1_v2 = direction.Dot(ray.direction);

    float32 det = v1_v2 * v1_v2 - v12 * v22;

    if (Math::Abs(det) > FLT_MIN)
    {
      float32 inv_det = 1.f / det;

      float32 dpv1 = dp.Dot(direction);
      float32 dpv2 = dp.Dot(ray.direction);

      float32 this_t = inv_det * (v22 * dpv1 - v1_v2 * dpv2);
        
      return GetPoint(-this_t);
    } else {
      Vector3 a = Vector3::Cross(dp, direction);
      return GetPoint(std::sqrt(a.Dot(a) / v12));
    }
  }

  //--------------------------------------------------------------
  String Ray::ToString() const {
    return StringUtils::Format("Origin: {}, Direction: {}", origin.ToString(), direction.ToString());
  }

  //--------------------------------------------------------------
  float32 Ray::DistanceBetweenRays(Ray a, Ray b) {
    Vector3 cross = Vector3::Cross(a.direction, b.direction);
    float32 magnitude = cross.Magnitude();
    if (magnitude == 0.0f) {
      return 0.0f;
    }

    Vector3 point = b.origin - a.origin;
    float32 dot = cross.Dot(point);

    return dot / magnitude;
  }

}
