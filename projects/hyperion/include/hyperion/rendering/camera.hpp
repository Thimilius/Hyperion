#pragma once

#include "hyperion/core/math/math.hpp"
#include "hyperion/core/math/ray.hpp"

namespace Hyperion::Rendering {

    enum class CameraMode {
        Perspective,
        Orthographic
    };

    class Camera {
    private:
        CameraMode m_mode = CameraMode::Perspective;
        
        Vec3 m_position = Vec3(0, 0, 0);
        Vec3 m_forward = Vec3(0, 0, -1);
        Vec3 m_up = Vec3(0, 1, 0);

        f32 m_near_plane = 0.1f;
        f32 m_far_plane = 100.0f;

        f32 m_fov = 90.0f;
        f32 m_size = 2.75f;

        Mat4 m_view_matrix;
        Mat4 m_projection_matrix;
        Mat4 m_view_projection_matrix;

        Mat4 m_inverse_view_matrix;
        Mat4 m_inverse_projection_matrix;
    public:
        inline CameraMode GetType() const { return m_mode; }
        inline void SetType(CameraMode type) { m_mode = type; }

        inline Vec3 GetPosition() const { return m_position; }
        inline void SetPosition(Vec3 position) { m_position = position; }
        inline Vec3 GetForward() const { return m_forward; }
        inline void SetForward(Vec3 forward) { m_forward = forward; }
        inline Vec3 GetUp() const { return m_up; }
        inline void SetUp(Vec3 up) { m_up = up; }

        inline f32 GetNearPlane() const { return m_near_plane; }
        inline void SetNearPlane(f32 near_plane) { m_near_plane = near_plane; }
        inline f32 GetFarPlane() const { return m_far_plane; }
        inline void SetFarPlane(f32 far_plane) { m_far_plane = far_plane; }

        inline f32 GetFOV() const { return m_fov; }
        inline void SetFOV(f32 fov) { m_fov = fov; }
        inline f32 GetSize() const { return m_size; }
        inline void SetSize(f32 size) { m_size = size; }

        Ray ScreenPointToRay(Vec2 screen_point);

        void RecalculateMatricies();

        const Mat4 &GetViewMatrix() const { return m_view_matrix; }
        const Mat4 &GetProjectionMatrix() const { return m_projection_matrix; }
        const Mat4 &GetViewProjectionMatrix() const { return m_view_projection_matrix; }

        const Mat4 &GetInverseViewMatrix() const { return m_inverse_view_matrix; }
        const Mat4 &GetInverseProjectionMatrix() const { return m_inverse_projection_matrix; }

        static Ref<Camera> Create();
    private:
        Camera() = default;
    };

}