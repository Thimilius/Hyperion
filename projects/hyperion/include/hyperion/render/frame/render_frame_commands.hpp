#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <variant>

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_types.hpp"
#include "hyperion/core/math/matrix4x4.hpp"
#include "hyperion/render/render_gizmos.hpp"
#include "hyperion/render/frame/buffer/render_frame_command_buffer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    enum class RenderFrameCommandType {
        SetCamera,

        ExecuteCommandBuffer,

        DrawMeshes,
        DrawGizmos
    };

    struct RenderFrameCommandSetCamera {
        uint64 camera_index;
    };

    struct RenderFrameCommandExecuteCommandBuffer {
        RenderFrameCommandBuffer command_buffer;
    };

    struct RenderFrameCommandDrawMeshes {

    };

    struct RenderFrameCommandDrawGizmos {
        AssetId shader_id;

        struct Grid {
            bool8 should_draw;
            Matrix4x4 local_to_world;
            RenderGizmoGridType type;
            AssetId mesh_id;
        } grid;
    };

    struct RenderFrameCommand {
        RenderFrameCommandType type;

        std::variant<
            RenderFrameCommandSetCamera,
            RenderFrameCommandExecuteCommandBuffer,
            RenderFrameCommandDrawMeshes,
            RenderFrameCommandDrawGizmos
        > data;
    };

}