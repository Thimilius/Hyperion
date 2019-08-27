#include "hyppch.hpp"

#include "hyperion/rendering/camera.hpp"

namespace Hyperion::Rendering {

    using namespace Math;

    void CPerspectiveCamera::Update() {
        float delta = CTime::GetDeltaTime();

        // Rotation
        {
            if (CInput::GetMouseButtonDown(EMouseButtonCode::Right) || CInput::GetMouseButtonDown(EMouseButtonCode::Middle)) {
                m_last_mouse_position = CInput::GetMousePosition();
            }

            float x_offset = 0.0f;
            float y_offset = 0.0f;
            if (CInput::GetMouseButton(EMouseButtonCode::Right) || CInput::GetMouseButton(EMouseButtonCode::Middle)) {
                SVec2 mouse_position = CInput::GetMousePosition();
                x_offset = mouse_position.x - m_last_mouse_position.x;
                y_offset = m_last_mouse_position.y - mouse_position.y;
                m_last_mouse_position.x = mouse_position.x;
                m_last_mouse_position.y = mouse_position.y;
            }

            float sensitivity = 0.25f;
            x_offset *= sensitivity;
            y_offset *= sensitivity;

            m_yaw += x_offset;
            m_pitch += y_offset;
            m_pitch = CMathf::Clamp(m_pitch, -89.0f, 89.0f);

            SVec3 forward = SVec3::Zero();
            forward.x = CMathf::Cos(CMathf::ToRadians(m_pitch)) * CMathf::Cos(CMathf::ToRadians(m_yaw));
            forward.y = CMathf::Sin(CMathf::ToRadians(m_pitch));
            forward.z = CMathf::Cos(CMathf::ToRadians(m_pitch)) * CMathf::Sin(CMathf::ToRadians(m_yaw));
            m_forward = forward.Normalized();
        }

        // Zoom
        {
            float wheel = CInput::GetMouseScroll();
            m_fov_target -= wheel * 500.0f * delta;
            m_fov_target = CMathf::Clamp(m_fov_target, 25, 120);
            m_fov = CMathf::Lerp(m_fov, m_fov_target, delta * 15);
        }

        // Movement
        {
            float camera_speed = m_speed * delta;
            if (CInput::GetKey(EKeyCode::Shift)) {
                camera_speed *= 2;
            }
            SVec3 direction = camera_speed * m_forward;
            SVec3 right = SVec3::Cross(m_forward, m_up).Normalized();
            right = camera_speed * right;
            SVec3 up = camera_speed * SVec3::Up();
            if (CInput::GetKey(EKeyCode::W)) {
                m_position = m_position + direction;
            }
            if (CInput::GetKey(EKeyCode::S)) {
                m_position = m_position - direction;
            }
            if (CInput::GetKey(EKeyCode::A)) {
                m_position = m_position - right;
            }
            if (CInput::GetKey(EKeyCode::D)) {
                m_position = m_position + right;
            }
            if (CInput::GetKey(EKeyCode::Control)) {
                m_position = m_position - up;
            }
            if (CInput::GetKey(EKeyCode::Space)) {
                m_position = m_position + up;
            }
        }

        // Reset
        if (CInput::GetKeyDown(EKeyCode::R)) {
            m_position = SVec3::Zero();
            m_forward = SVec3(0, 0, -1);
            m_up = SVec3::Up();

            m_pitch = 0.0f;
            m_yaw = -90.0f;

            m_fov = 90.0f;
            m_fov_target = m_fov;
        }

        RecacluateMatricies();
    }

    void CPerspectiveCamera::RecacluateMatricies() {
        CWindow *window = CApplication::GetInstance()->GetWindow();
        float aspect_ratio = (float)window->GetWidth() / (float)window->GetHeight();
        m_view_matrix = SMat4::LookAt(m_position, m_position + m_forward, m_up);
        m_projection_matrix = SMat4::Perspective(m_fov, aspect_ratio, 0.1f, 10.0f);
        m_view_projection_matrix = m_projection_matrix * m_view_matrix;
    }

}