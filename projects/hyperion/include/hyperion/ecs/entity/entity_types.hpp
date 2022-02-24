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

  using EntityId = uint64;
  using EntityIndex = uint32;
  using EntityVersion = uint32;
  using EntityGuid = Guid;

  using EntityCallback = Delegate<void(World *, EntityId)>;

  struct EntityDescription {
    EntityId id;
    EntityGuid guid;
  };

  namespace Entity {
    inline static constexpr EntityId EMPTY = 0xFFFFFFFF;
  }

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