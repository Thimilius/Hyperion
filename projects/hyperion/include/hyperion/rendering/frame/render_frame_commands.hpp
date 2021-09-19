#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_types.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/matrix4x4.hpp"
#include "hyperion/rendering/gizmos/render_gizmo_types.hpp"
#include "hyperion/rendering/types/render_types_general.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    enum class RenderFrameCommandType {
        SetCamera,

        Clear,

        DrawAll,
        DrawGizmos
    };

    struct RenderFrameCommand {
        RenderFrameCommandType type;

        union Data {
            struct SetCamera {
                uint64 camera_index;
            } set_camera;
            struct Clear {
                ClearFlags flags;
                Color color;
            } clear;
            struct DrawAll {

            } draw_all;
            struct DrawGizmos {
                struct Grid {
                    bool8 should_draw;
                    Matrix4x4 local_to_world;
                    RenderGizmoGridType type;
                    AssetId mesh_id;
                } grid;
            } draw_gizmos;
        } data = { };
    };

}