#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/guid.hpp"
#include "hyperion/ecs/component/component_registry.hpp"
#include "hyperion/ecs/entity/entity_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    using EntityIndices = List<uint32>;
    using EntityIndexPages = List<EntityIndices>;
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

        void FitIntoPool(EntityId id);
    private:
        uint32 GetSparseIndex(EntityId id);
        List<uint32> &GetEntityIndices(EntityId id);
    public:
        inline static const uint32 MAX_ENTITIES_PER_PAGE = 2048;
    private:
        EntityIndexPages m_entity_index_pages;
        EntityList m_entity_list;
        ComponentList m_component_list;

        ComponentInfo m_component_info;
    private:
        inline static const uint32 SPARSE_ELEMENT = 0xFFFFFFFF;
    };

}