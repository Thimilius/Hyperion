//------------------------ Header Guard ------------------------
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

    inline static constexpr uint32 CreateSmallId(EntityId id) { return (0x00FF & GetIndex(id)) | (0xFF00 & GetVersion(id)); }
    inline static constexpr EntityId CreateFromSmallId(uint32 small_id) { return CreateId(0x00FF & small_id, 0xFF00 & small_id); }
  };

}