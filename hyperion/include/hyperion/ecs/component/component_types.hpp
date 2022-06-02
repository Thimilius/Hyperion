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

  using ComponentConstructor = void *(*)(void *address);
  using ComponentCopyConstructor = void *(*)(void *address, const void *instance);
  using ComponentMoveAssignmentOperator = void(*)(void *destination_instance, void *source_instance);
  using ComponentDestructor = void(*)(const void *instance);
  
  struct ComponentInfo {
    ComponentId id;

    Type *type;
    uint64 element_size;

    ComponentConstructor constructor;
    ComponentCopyConstructor copy_constructor;
    ComponentMoveAssignmentOperator move_assignment_operator;
    ComponentDestructor destructor;
  };

  using ComponentCallback = Delegate<void(EntityManager *, EntityId)>;

  struct ComponentCallbacks {
    Array<ComponentCallback> added;
    Array<ComponentCallback> removed;
  };

}
