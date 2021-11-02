//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Threading {
    
    using ThreadId = uint32;
    using ThreadStartFunction = std::function<void()>;
    using ParameterizedThreadStartFunction = std::function<void(void *)>;

}