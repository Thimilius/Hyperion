#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <bitset>

//---------------------- Project Includes ----------------------
#include "hyperion/core/guid.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    using EntityId = uint64;
    using EntityIndex = uint32;
    using EntityVersion = uint32;
    using EntityGuid = Guid;

    const uint32 MAX_ENTITY_COMPONENTS = 32;

    using EntityComponentMask = std::bitset<MAX_ENTITY_COMPONENTS>;

    struct EntityDescription {
        EntityId id;
        EntityGuid guid;

        // TEMP: This is just used as long as we do not implement sparse sets.
        EntityComponentMask component_mask;
    };

    class Entity {
    public:
        inline static constexpr EntityId CreateId(EntityIndex index, EntityVersion version) { return (static_cast<EntityId>(index) << 32) | static_cast<EntityId>(version); }
        inline static EntityGuid CreateGuid() { return Guid::Create(); }

        inline static constexpr EntityIndex GetIndex(EntityId id) { return id >> 32; }
        inline static constexpr EntityVersion GetVersion(EntityId id) { return static_cast<EntityVersion>(id); }
    public:
        inline static constexpr EntityId EMPTY = 0xFFFFFFFF00000000;
    };

}