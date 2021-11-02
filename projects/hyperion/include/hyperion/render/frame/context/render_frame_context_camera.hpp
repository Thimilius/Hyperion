//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/matrix4x4.hpp"
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/render/types/render_types_camera.hpp"
#include "hyperion/render/types/render_types_general.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct RenderFrameContextCamera {
        CameraProjectionMode projection_mode;

        CameraClearMode clear_mode;
        Color background_color;

        LayerMask culling_mask;

        Vector3 position;
        Vector3 forward;
        Vector3 up;

        float32 near_plane;
        float32 far_plane;

        float32 fov;
        float32 orthographic_size;

        CameraViewport viewport;

        Matrix4x4 view_matrix;
        Matrix4x4 projection_matrix;
        Matrix4x4 view_projection_matrix;

        Matrix4x4 inverse_view_matrix;
        Matrix4x4 inverse_projection_matrix;
        Matrix4x4 inverse_view_projection_matrix;
    };

}