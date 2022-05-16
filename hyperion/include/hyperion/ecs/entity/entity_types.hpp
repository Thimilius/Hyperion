//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/delegate.hpp"
#include "hyperion/core/guid.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class World;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {
  
  using EntityIdType = uint64;
  using EntityIndex = uint32;
  using EntityVersion = uint32;
  using EntityGuid = Guid;

  struct EntityId {
    uint64 id;

    EntityId() : id(0xFFFFFFFF) { }
    EntityId(EntityIdType id) : id(id) { }

    bool8 operator==(const EntityId &other) const { return id == other.id; }
    bool8 operator!=(const EntityId &other) const { return !(*this == other); }
    operator uint64() const { return id; }
  };

  namespace Entity {
    inline static const EntityId EMPTY = { 0xFFFFFFFF };
  }

  using EntityCallback = Delegate<void(World *, EntityId)>;

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
  
  template <>
    struct std::formatter<Hyperion::EntityId> : std::formatter<Hyperion::EntityIdType> {
    auto format(Hyperion::EntityId id, std::format_context& ctx) {
      return std::formatter<Hyperion::EntityIdType>::format(id.id, ctx);
    }
  };

  template<>
  struct hash<Hyperion::EntityId> {
    std::size_t operator()(const Hyperion::EntityId &id) const {
      return hash<Hyperion::EntityIdType>()(id.id);
    }
  };
  
}
