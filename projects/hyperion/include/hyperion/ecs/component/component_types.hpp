#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    using ComponentId = uint32;
    using ComponentDestructorFunction = void(*)(const void *);

    struct ComponentInfo {
        ComponentId id;
        uint64 element_size;
        ComponentDestructorFunction destructor;
    };

}