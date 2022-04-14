//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/system/system.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Physics {

  class UpdateColliderTransformSystem : public ISystem {
  public:
    void Run(World *world) override;
  };

}
