#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/entity/entity_types.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    struct HierarchyComponent;
    class World;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class WorldHierarchyTransformUpdate {
        None,
        Branch,
        All,
    };

    class WorldHierarchy final {
    public:
        const Array<EntityId> &GetRoots() const { return m_roots; }

        void SetParent(EntityId entity, EntityId parent, WorldHierarchyTransformUpdate update = WorldHierarchyTransformUpdate::None);

        void UpdateTransform(WorldHierarchyTransformUpdate update = WorldHierarchyTransformUpdate::Branch, EntityId branch = Entity::EMPTY);
    private:
        void AddRoot(EntityId entity, HierarchyComponent *entity_hierarchy);
    private:
        World *m_world;
        Array<EntityId> m_roots;
    private:
        friend class Hyperion::World;
    };

}