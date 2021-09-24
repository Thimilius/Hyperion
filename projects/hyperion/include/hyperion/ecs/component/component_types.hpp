#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/component.hpp"
#include "hyperion/ecs/entity/entity_types.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class World;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    using ComponentId = uint32;

    struct ComponentInfo {
        ComponentId id;

        MetaType type;
        uint64 element_size;

        MetaTrivialDestructor destructor;
        MetaInPlaceConstructor constructor;
    };

    using ComponentCallback = Delegate<void(World *, EntityId)>;
    
    struct ComponentCallbacks {
        Array<ComponentCallback> added;
        Array<ComponentCallback> removed;
    };

}