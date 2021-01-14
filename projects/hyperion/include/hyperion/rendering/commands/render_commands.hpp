#pragma once

#include "hyperion/rendering/render_driver.hpp"

namespace Hyperion::Rendering {

    enum class RenderCommandType {
        Exit = -1,

        Clear,

        CreateMesh,
        FreeMesh
    };

    struct RenderCommandExit { };

    struct RenderCommandClear {
        ClearFlags clear_flags;
        Color color;
    };

    struct RenderCommandCreateMesh {
        ResourceId id;
        Vector<VertexMesh> verticies;
        Vector<u32> indicies;
    };

}