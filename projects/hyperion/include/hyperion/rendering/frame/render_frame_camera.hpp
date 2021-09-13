#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/color.hpp"
#include "hyperion/core/math/matrix4x4.hpp"
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/rendering/render_types.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct RenderFrameCamera {
        CameraProjectionMode projection_mode = CameraProjectionMode::Perspective;

        CameraClearMode clear_mode = CameraClearMode::Color;
        Color background_color = Color::Black();

        Vector3 position = Vector3(0, 0, 0);
        Vector3 forward = Vector3(0, 0, -1);
        Vector3 up = Vector3(0, 1, 0);

        float32 near_plane = 0.1f;
        float32 far_plane = 1000.0f;

        float32 fov = 90.0f;
        float32 orthographic_size = 2.75f;

        Matrix4x4 view_matrix = Matrix4x4::Identity();
        Matrix4x4 projection_matrix = Matrix4x4::Identity();
        Matrix4x4 view_projection_matrix = Matrix4x4::Identity();

        Matrix4x4 inverse_view_matrix = Matrix4x4::Identity();
        Matrix4x4 inverse_projection_matrix = Matrix4x4::Identity();
        Matrix4x4 inverse_view_projection_matrix = Matrix4x4::Identity();

        CameraViewport viewport;
    };

}