//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/system/system.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class AssetLoadSystem final : public ISystem {
  public:
    void Run(EntityManager *manager) override;
  };

  class AssetUnloadSystem final : public ISystem {
  public:
    void Run(EntityManager *manager) override;
  };

}
