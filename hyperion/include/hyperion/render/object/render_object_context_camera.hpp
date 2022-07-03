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

  struct RenderObjectContextCamera {
    uint32 index = 0;

    CameraProjectionMode projection_mode = CameraProjectionMode::Perspective;

    CameraClearMode clear_mode = CameraClearMode::Color;
    Color background_color;

    LayerMask culling_mask = LayerMask::Everything;

    Vector3 position;
    Vector3 forward;
    Vector3 up;

    float32 near_plane = 0.1f;
    float32 far_plane = 100.0f;

    float32 fov = 60.0f;
    float32 orthographic_size = 10.0f;

    CameraViewport viewport = { };

    Matrix4x4 view_matrix;
    Matrix4x4 projection_matrix;
    Matrix4x4 view_projection_matrix;

    Matrix4x4 inverse_view_matrix;
    Matrix4x4 inverse_projection_matrix;
    Matrix4x4 inverse_view_projection_matrix;
  };

}
