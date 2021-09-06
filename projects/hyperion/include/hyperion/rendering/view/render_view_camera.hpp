#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/matrix4x4.hpp"
#include "hyperion/core/math/vector3.hpp"
#include "hyperion/rendering/camera_projection_mode.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    struct RenderViewCameraData {
        CameraProjectionMode projection_mode = CameraProjectionMode::Perspective;

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
    };

    class RenderViewCamera {
    public:
        RenderViewCameraData &GetData() { return m_data; }
        const RenderViewCameraData &GetData() const { return m_data; }
    private:
        RenderViewCameraData m_data;
    };

}