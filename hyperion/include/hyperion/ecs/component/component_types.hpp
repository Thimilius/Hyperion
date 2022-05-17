//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/entity/entity_types.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class EntityManager;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  using ComponentId = uint32;

  using ComponentConstructor = void *(*)(void *);
  using ComponentDestructor = void(*)(const void *);
  
  struct ComponentInfo {
    ComponentId id;

    Type *type;
    uint64 element_size;

    ComponentConstructor constructor;
    ComponentDestructor destructor;
  };

  using ComponentCallback = Delegate<void(EntityManager *, EntityId)>;

  struct ComponentCallbacks {
    Array<ComponentCallback> added;
    Array<ComponentCallback> removed;
  };

}
