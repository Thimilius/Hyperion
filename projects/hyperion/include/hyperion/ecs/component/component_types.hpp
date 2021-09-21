#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/entity/entity_types.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class World;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    using ComponentId = uint32;
    using ComponentDestructorFunction = void(*)(const void *);

    struct ComponentInfo {
        String name;
        ComponentId id;
        uint64 element_size;
        ComponentDestructorFunction destructor;
    };

    using ComponentCallback = Delegate<void(World *, EntityId)>;
    
    struct ComponentCallbacks {
        Array<ComponentCallback> added;
        Array<ComponentCallback> removed;
    };

}