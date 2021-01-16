#pragma once

#include "hyperion/rendering/render_driver.hpp"

namespace Hyperion::Rendering {

    enum class RenderCommandType {
        Exit = -1,

        Clear,
        Viewport,

        CreateShader,
        FreeShader,

        CreateMesh,
        FreeMesh,

        DrawIndexed
    };

    struct RenderCommandClear {
        ClearFlags clear_flags;
        Color color;
    };

    struct RenderCommandViewport {
        Viewport viewport;
    };

    struct RenderCommandId {
        ResourceId id;
    };

    struct RenderCommandCreateShader {
        ResourceId id;
        ShaderDescriptor descriptor;
    };

    struct RenderCommandCreateMesh {
        ResourceId id;
        MeshDescriptor descriptor;
    };

    struct RenderCommandDrawIndexed {
        ResourceId shader_id;
        ResourceId mesh_id;
    };

}