#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/entity/entity_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class EntityUtilities final {
    public:
        inline static constexpr EntityId CreateId(EntityIndex index, EntityVersion version) { return static_cast<EntityId>(index) | (static_cast<EntityId>(version) << 32); }
        inline static EntityGuid CreateGuid() { return Guid::Generate(); }

        inline static constexpr EntityIndex GetIndex(EntityId id) { return static_cast<EntityIndex>(id); }
        inline static constexpr EntityVersion GetVersion(EntityId id) { return id >> 32; }
    };

}