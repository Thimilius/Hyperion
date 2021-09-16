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

    enum class WorldHierarchyDestructionPolicy {
        DestroyChildren,
        KeepChildrenAsRoots,
    };

    class WorldHierarchy final {
    public:
        const Array<EntityId> &GetRoots() const { return m_roots; }

        void SetParent(EntityId entity, EntityId parent, WorldHierarchyTransformUpdate update = WorldHierarchyTransformUpdate::None);

        void UpdateTransform(WorldHierarchyTransformUpdate update = WorldHierarchyTransformUpdate::Branch, EntityId branch = Entity::EMPTY);
    private:
        void HandleEntityCreation(EntityId entity);
        void HandleEntityDestruction(EntityId entity, WorldHierarchyDestructionPolicy destruction_policy);

        void RemoveOldRelations(EntityId entity, HierarchyComponent *entity_hierarchy);
        void AddRootRelation(EntityId entity, HierarchyComponent *entity_hierarchy);
    private:
        World *m_world;

        // TODO: Make this an implicit list as well.
        Array<EntityId> m_roots;
    private:
        friend class Hyperion::World;
    };

}