#pragma once

#include "hyperion/rendering/command_buffer.hpp"

namespace Hyperion::Rendering {

    enum class OpenGLCommandType {
        Clear,
        SetViewport,
        SetRasterizerState,

        SetupCameraData,

        SetRenderTexture,
        Blit,

        DrawMesh,
    };

    struct OpenGLCommandClear {
        ClearFlags clear_flags;
        Color color;
    };

    struct OpenGLCommandSetViewport {
        Viewport viewport;
    };

    struct OpenGLCommandSetRasterizerState {
        RasterizerState rasterizer_state;
    };

    struct OpenGLCommandSetupCameraData {
        CameraData camera_data;
    };

    struct OpenGLCommandSetRenderTexture {
        ResourceId render_texture_id;
    };

    // TODO: Add support for full destination/source rectangle bounds.
    struct OpenGLCommandBlit {
        ResourceId destination_id;
        uint32 destination_width;
        uint32 destination_height;
        ResourceId source_id;
        uint32 source_width;
        uint32 source_height;
    };

    struct OpenGLCommandDrawMesh {
        ResourceId mesh_id;
        Mat4 model_matrix;
        ResourceId material_id;
        uint32 sub_mesh_index;
    };

}
