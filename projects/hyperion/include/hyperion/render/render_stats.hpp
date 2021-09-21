#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct RenderStats {
        uint64 draw_calls = 0;
        uint64 triangle_count = 0;
        uint64 vertex_count = 0;
    };

}