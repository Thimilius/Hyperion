#pragma once

#include "hyperion/rendering/command_buffer.hpp"

namespace Hyperion::Rendering {

    enum class OpenGLCommandType {
        Clear,
        SetViewport,

        DrawMesh,
    };

    struct OpenGLCommandClear {
        ClearFlags clear_flags;
        Color color;
    };

    struct OpenGLCommandSetViewport {
        Viewport viewport;
    };

    struct OpenGLCommandDrawMesh {
        ResourceId mesh_id;
        ResourceId material_id;
        uint32 sub_mesh_index;
    };

}
