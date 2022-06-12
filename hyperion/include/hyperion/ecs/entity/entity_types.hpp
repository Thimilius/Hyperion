//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/delegate.hpp"
#include "hyperion/core/guid.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class EntityManager;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {
  
  using EntityIdType = uint64;
  using EntityIdSmallType = uint32;
  using EntityIndex = uint32;
  using EntityVersion = uint32;
  using EntityGuid = Guid;

  struct EntityId {
    EntityIdType id = EMPTY;

    EntityId() = default;
    EntityId(EntityIdType id) : id(id) { }

    bool8 operator==(const EntityId &other) const { return id == other.id; }
    bool8 operator!=(const EntityId &other) const { return !(*this == other); }
    bool8 operator==(const EntityIdType &other) const { return id == other; }
    bool8 operator!=(const EntityIdType &other) const { return !(*this == other); }
    
    operator EntityIdType() const { return id; }

    inline static constexpr EntityIdType EMPTY = 0xFFFFFFFFFFFFFFFF;

    inline static EntityId Create(EntityIndex index, EntityVersion version) {
      return static_cast<EntityId>(index).id | (static_cast<EntityId>(version).id << 32);
    }

    inline static EntityIndex GetIndex(EntityId id) { return static_cast<EntityIndex>(id.id); }
    inline static EntityVersion GetVersion(EntityId id) { return id.id >> 32; }

    inline static EntityIdSmallType CreateSmall(EntityId id) { return (0x0000FFFF & GetIndex(id)) | (GetVersion(id) << 16); }
    inline static EntityId FromSmall(EntityIdSmallType small_id) { return Create(0x0000FFFF & small_id, small_id >> 16); }    
  };

  using EntityCallback = Delegate<void(EntityManager *, EntityId)>;

  struct EntityDescription {
    EntityId id;
    EntityGuid guid;
  };

  enum class EntityPrimitive {
    Empty,
    Base,

    Camera,

    DirectionalLight,
    PointLight,
    SpotLight,

    Quad,
    Plane,
    Cube,
    Sphere,

    Sprite
  };

}

namespace std {
  
  template<>
  struct std::formatter<Hyperion::EntityId> : std::formatter<Hyperion::EntityIdType> {
    auto format(Hyperion::EntityId id, std::format_context& ctx) {
      return std::formatter<Hyperion::EntityIdType>::format(id.id, ctx);
    }
  };

  template<>
  struct hash<Hyperion::EntityId> {
    std::size_t operator()(const Hyperion::EntityId &id) const noexcept {
      return hash<Hyperion::EntityIdType>()(id.id);
    }
  };
  
}
