#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/guid.hpp"
#include "hyperion/ecs/entity/entity_types.hpp"
#include "hyperion/ecs/component/component.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    using EntityIndices = List<uint32>;
    using EntityList = List<EntityId>;
    using ComponentList = List<byte>;

    class ComponentPool final {
    public:
        ComponentPool(uint64 max_entities, ComponentInfo component_info);
    public:
        inline uint64 GetEntityCount() const { return m_entity_list.GetLength(); }
        inline EntityId GetEntity(EntityIndex index) const { return m_entity_list[index]; }

        byte *AddComponent(EntityId id);
        bool HasComponent(EntityId id);
        byte *GetComponent(EntityId id);
        bool RemoveComponent(EntityId id);
    private:
        EntityIndices m_entity_indices;
        EntityList m_entity_list;
        ComponentList m_component_list;

        ComponentInfo m_component_info;
    private:
        inline static const uint32 SPARSE_ELEMENT = 0xFFFFFFFF;
    };

}