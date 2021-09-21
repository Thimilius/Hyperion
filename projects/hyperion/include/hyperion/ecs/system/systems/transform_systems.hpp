#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/system/system.hpp"
#include "hyperion/ecs/component/components/transform_components.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class HierarchyTransformSystem : public ISystem {
    public:
        void Run(World *world) override;

        void UpdateBranch(World *world, EntityId root, HierarchyComponent *root_hierarchy, DerivedTransformComponent *parent_derived_transform);
    };

    class LocalToWorldSystem : public ISystem {
    public:
        void Run(World *world) override;
    };

}