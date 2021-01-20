#pragma once

#include "hyperion/rendering/render_driver.hpp"

// FIXME: Currently we have a very heavy memory leak as destructors for the render commands do not get called

namespace Hyperion::Rendering {

    enum class RenderCommandType {
        Exit = -1,

        Clear,
        Viewport,
        SetRasterizerState,

        CreateShader,
        FreeShader,

        CreateMesh,
        FreeMesh,

        CreateTexture,
        FreeTexture,

        DrawIndexed
    };

    struct RenderCommandClear {
        ClearFlags clear_flags;
        Color color;
    };

    struct RenderCommandViewport {
        Viewport viewport;
    };

    struct RenderCommandSetRasterizerState {
        RasterizerState rasterizer_state;
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

    struct RenderCommandCreateTexture {
        ResourceId id;
        TextureDescriptor descriptor;
    };

    struct RenderCommandDrawIndexed {
        ResourceId shader_id;
        ResourceId mesh_id;
    };

}