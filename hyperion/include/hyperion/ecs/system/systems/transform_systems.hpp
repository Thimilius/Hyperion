//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/system/system.hpp"
#include "hyperion/ecs/component/components/transform_components.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class HierarchyTransformSystem final : public ISystem {
  public:
    void Run(EntityManager *manager) override;
  public:
    static void UpdateBranch(EntityManager *manager, EntityId branch, HierarchyComponent *branch_hierarchy,
                             DerivedTransformComponent *parent_derived_transform);
  };

  class LocalToWorldSystem final : public ISystem {
  public:
    void Run(EntityManager *manager) override;
  };

}
