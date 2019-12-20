#pragma once

#include "hyperion/core/math/math.hpp"

namespace Hyperion::Rendering {

    class Camera {
    protected:
        Vec3 m_position;
        Vec3 m_forward;
        Vec3 m_up;

        f32 m_near_plane = 0.1f;
        f32 m_far_plane = 100.0f;

        Mat4 m_view_matrix;
        Mat4 m_projection_matrix;
        Mat4 m_view_projection_matrix;
    public:
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

        virtual void RecalculateMatricies() = 0;

        const Mat4 &GetViewMatrix() const { return m_view_matrix; }
        const Mat4 &GetProjectionMatrix() const { return m_projection_matrix; }
        const Mat4 &GetViewProjectionMatrix() const { return m_view_projection_matrix; }
    };

    class PerspectiveCamera : public Camera {
    private:
        f32 m_fov;
    public:
        inline f32 GetFOV() const { return m_fov; }
        inline void SetFOV(f32 fov) { m_fov = fov; }

        void RecalculateMatricies() override;
    };

    class OrthographicCamera : public Camera {
    private:
        f32 m_size;
    public:
        inline f32 GetSize() const { return m_size; }
        inline void SetSize(f32 size) { m_size = size; }

        void RecalculateMatricies() override;
    };

}