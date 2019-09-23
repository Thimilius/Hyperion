#pragma once

#include "hyperion/math/math.hpp"

namespace Hyperion::Rendering {

    class CCamera {
    protected:
        Math::SVec3 m_position;
        Math::SVec3 m_forward;
        Math::SVec3 m_up;

        float m_near_plane = 0.1f;
        float m_far_plane = 100.0f;

        Math::SMat4 m_view_matrix;
        Math::SMat4 m_projection_matrix;
        Math::SMat4 m_view_projection_matrix;
    public:
        inline Math::SVec3 GetPosition() const { return m_position; }
        inline void SetPosition(Math::SVec3 position) { m_position = position; }

        inline Math::SVec3 GetForward() const { return m_forward; }
        inline void SetForward(Math::SVec3 forward) { m_forward = forward; }

        inline Math::SVec3 GetUp() const { return m_up; }
        inline void SetUp(Math::SVec3 up) { m_up = up; }

        inline float GetNearPlane() const { return m_near_plane; }
        inline void SetNearPlane(float near_plane) { m_near_plane = near_plane; }

        inline float GetFarPlane() const { return m_far_plane; }
        inline void SetFarPlane(float far_plane) { m_far_plane = far_plane; }

        virtual void RecalculateMatricies() = 0;

        const Math::SMat4 &GetViewMatrix() const { return m_view_matrix; }
        const Math::SMat4 &GetProjectionMatrix() const { return m_projection_matrix; }
        const Math::SMat4 &GetViewProjectionMatrix() const { return m_view_projection_matrix; }
    };

    class CPerspectiveCamera : public CCamera {
    private:
        float m_fov;
    public:
        inline float GetFOV() const { return m_fov; }
        inline void SetFOV(float fov) { m_fov = fov; }

        void RecalculateMatricies() override;
    };
}