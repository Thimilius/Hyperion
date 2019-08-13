#pragma once

#include "hyperion/math/math.hpp"

namespace Hyperion {

    class CCamera {
    protected:
        Math::SMat4 m_view_matrix;
        Math::SMat4 m_projection_matrix;
        Math::SMat4 m_view_projection_matrix;
    public:
        const Math::SMat4 &GetViewMatrix() const { return m_view_matrix; }
        const Math::SMat4 &GetProjectionMatrix() const { return m_projection_matrix; }
        const Math::SMat4 &GetViewProjectionMatrix() const { return m_view_projection_matrix; }
    };

    class CPerspectiveCamera : public CCamera {
    private:
        Math::SVec3 m_position = Math::SVec3(0, 0, 1);
        Math::SVec3 m_forward = Math::SVec3(0, 0, -1);
        Math::SVec3 m_up = Math::SVec3::Up();

        float m_fov = 90.0f;
        float m_fov_target = m_fov;

        float m_speed = 5.0f;

        float m_yaw = -90.0f;
        float m_pitch = 0.0f;

        Math::SVec2 m_last_mouse_position;
    public:
        void Update();
    private:
        void RecacluateMatricies();
    };
}