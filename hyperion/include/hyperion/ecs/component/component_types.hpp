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
  using ComponentGuid = Guid;

  using ComponentConstructor = void *(*)(void *address);
  using ComponentCopyConstructor = void *(*)(void *address, const void *instance);
  using ComponentCopyAssignmentOperator = void(*)(void *destination_instance, void *source_instance);
  using ComponentMoveAssignmentOperator = void(*)(void *destination_instance, void *source_instance);
  using ComponentDestructor = void(*)(const void *instance);
  
  struct ComponentInfo {
    ComponentId id;
    ComponentGuid guid;

    Type *type;
    uint64 element_size;

    ComponentConstructor constructor;
    ComponentCopyConstructor copy_constructor;
    ComponentCopyAssignmentOperator copy_assignment_operator;
    ComponentMoveAssignmentOperator move_assignment_operator;
    ComponentDestructor destructor;
  };

  using ComponentCallback = Delegate<void(EntityManager *, ComponentId, EntityId)>;

  struct ComponentCallbacks {
    Array<ComponentCallback> added;
    Array<ComponentCallback> removed;
  };

}
