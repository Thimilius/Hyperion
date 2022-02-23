//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/ray.hpp"
#include "hyperion/core/math/vector3.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  struct Plane {
    Vector3 normal;
    float32 distance;

    Plane();
    Plane(Vector3 normal, float32 distance);
    Plane(Vector3 normal, Vector3 point);
    Plane(Vector3 a, Vector3 b, Vector3 c);

    void Flip();

    Vector3 GetClosestPoint(Vector3 point) const;
    float32 GetDistanceToPoint(Vector3 point) const;

    bool8 Intersects(Ray ray, float32 &hit_distance) const;

    String ToString() const;

    bool8 operator==(const Plane &other) const;
    bool8 operator!=(const Plane &other) const;
  };

}