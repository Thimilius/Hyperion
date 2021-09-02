#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/guid.hpp"
#include "hyperion/ecs/component/component_registry.hpp"
#include "hyperion/ecs/entity/entity_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    using EntityList = List<EntityId>;
    using ComponentList = List<byte>;

    class ComponentPool final {
    public:
        ComponentPool(ComponentInfo component_info);
    public:
        inline uint64 GetEntityCount() const { return m_entity_list.GetLength(); }
        inline EntityId GetEntity(EntityIndex index) const { return m_entity_list[index]; }

        byte *AddComponent(EntityId id);
        bool HasComponent(EntityId id);
        byte *GetComponent(EntityId id);
        bool RemoveComponent(EntityId id);
    private:
        List<uint32> m_entity_indices;
        EntityList m_entity_list;
        ComponentList m_component_list;

        ComponentInfo m_component_info;
    private:
        inline static const uint32 MAX_ENTITIES = 4096;
        inline static const uint32 SPARSE_ELEMENT = 0xFFFFFFFF;
    };

}