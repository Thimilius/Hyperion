#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"
#include "hyperion/rendering/types/render_types_general.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    enum class RenderFrameCommandType {
        SetCamera,

        Clear,

        DrawAll
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
        } data = { };
    };

}