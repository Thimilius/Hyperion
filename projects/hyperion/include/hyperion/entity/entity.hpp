#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/guid.hpp"
#include "hyperion/entity/component.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class World;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    using EntityId = uint64;
    using EntityIndex = uint32;
    using EntityVersion = uint32;
    using EntityGuid = Guid;

    struct EntityDescription {
        EntityId id;
        EntityGuid guid;
    };

    class Entity final {
    public:
        inline static constexpr EntityId CreateId(EntityIndex index, EntityVersion version) { return static_cast<EntityId>(index) | (static_cast<EntityId>(version) << 32); }
        inline static EntityGuid CreateGuid() { return Guid::Create(); }

        inline static constexpr EntityIndex GetIndex(EntityId id) { return static_cast<EntityIndex>(id); }
        inline static constexpr EntityVersion GetVersion(EntityId id) { return id >> 32; }
    public:
        inline static constexpr EntityId EMPTY = 0xFFFFFFFF00000000;
    };

    using EntityIndices = List<uint32>;
    using EntityList = List<EntityId>;
    using ComponentList = List<byte>;

    class EntityPool final {
    public:
        EntityPool(uint64 max_entities, ComponentId component_id, uint64 component_element_size);
    public:
        byte *AddComponent(EntityId id);
        byte *GetComponent(EntityId id);
        void RemoveComponent(EntityId id);
    private:
        EntityIndices m_entity_indices;
        EntityList m_entity_list;
        ComponentList m_component_list;

        ComponentId m_component_id;
        uint64 m_component_element_size;
    private:
        inline static const uint32 SPARSE_ELEMENT = 0xFFFFFFFF;
    private:
        friend class Hyperion::World;
    };

}