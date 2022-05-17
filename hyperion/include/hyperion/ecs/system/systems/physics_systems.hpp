//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/system/system.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Physics {

  class UpdateColliderTransformSystem final : public ISystem {
  public:
    void Run(EntityManager *manager) override;
  };

}
