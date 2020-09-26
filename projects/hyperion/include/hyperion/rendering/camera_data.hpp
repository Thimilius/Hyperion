#pragma once

#include "hyperion/core/math/mat4.hpp"
#include "hyperion/core/math/ray.hpp"

namespace Hyperion::Rendering {

    enum class CameraMode {
        Perspective,
        Orthographic
    };

    struct CameraData {
        CameraMode mode = CameraMode::Perspective;

        Vec3 position = Vec3(0, 0, 0);
        Vec3 forward = Vec3(0, 0, -1);
        Vec3 up = Vec3(0, 1, 0);

        f32 near_plane = 0.1f;
        f32 far_plane = 1000.0f;

        f32 fov = 90.0f;
        f32 orthographic_size = 2.75f;

        Mat4 view_matrix;
        Mat4 projection_matrix;
        Mat4 view_projection_matrix;

        Mat4 inverse_view_matrix;
        Mat4 inverse_projection_matrix;
        Mat4 inverse_view_projection_matrix;
    };

}