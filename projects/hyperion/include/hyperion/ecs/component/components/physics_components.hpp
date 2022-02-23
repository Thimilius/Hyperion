//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/ecs/component/component.hpp"

namespace Hyperion::Physics {

  struct BoxColliderComponent : public IComponent {
    Vector3 origin = Vector3::Zero();
    Vector3 size = Vector3::One();
  };

  struct SphereColliderComponent : public IComponent {
    float32 radius = 0.5f;
  };

}