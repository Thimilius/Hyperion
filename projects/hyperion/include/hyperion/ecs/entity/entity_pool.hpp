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
    using ComponentDestructorFunction = void(*)(const void *);

    class EntityPool final {
    public:
        EntityPool(uint64 max_entities, ComponentId component_id, uint64 component_element_size, ComponentDestructorFunction component_destructor);
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

        ComponentId m_component_id;
        uint64 m_component_element_size;
        ComponentDestructorFunction m_component_destructor = nullptr;
    private:
        inline static const uint32 SPARSE_ELEMENT = 0xFFFFFFFF;
    };

}