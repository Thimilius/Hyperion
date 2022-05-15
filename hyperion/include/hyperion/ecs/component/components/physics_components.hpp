//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/vector3.hpp"

namespace Hyperion::Physics {

  struct BoxColliderComponent {
    Vector3 origin = Vector3::Zero();
    Vector3 size = Vector3::One();
  };

  struct SphereColliderComponent {
    float32 radius = 0.5f;
  };

}
