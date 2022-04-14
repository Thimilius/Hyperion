//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/entity/entity_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Physics {

  enum class ColliderType {
    BoxCollider,
    SphereCollider
  };

  struct RaycastResult {
    Vector3 point = Vector3::Zero();
    Vector3 normal = Vector3::Zero();

    EntityId entity;
  };

}
