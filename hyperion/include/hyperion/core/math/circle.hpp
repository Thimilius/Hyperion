//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  struct Circle {
    Vector3 center;
    Vector3 orientation;
    float32 radius;

    Circle();
    Circle(Vector3 center, Vector3 orientation, float32 radius);

    float32 GetClosestPointToRay(Ray ray, Vector3 &point);
    float32 GetAngleBetweenPoints(Vector3 a, Vector3 b);
  };  
  
}
  