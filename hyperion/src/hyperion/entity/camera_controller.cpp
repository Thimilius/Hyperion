#include "hyppch.hpp"

#include "hyperion/entity/camera_controller.hpp"
#include "hyperion/app/input.hpp"

using namespace Hyperion::Math;
using namespace Hyperion::Rendering;

namespace Hyperion::Entity {

    CPerspectiveCameraController::CPerspectiveCameraController(const TRef<CPerspectiveCamera> &perspective_camera) {
        m_perspective_camera = perspective_camera;

        m_perspective_camera->SetPosition(SVec3(0, 0, 1));
        m_perspective_camera->SetForward(SVec3(0, 0, -1));
        m_perspective_camera->SetUp(SVec3(0, 1, 0));

        m_perspective_camera->SetFOV(90.0f);
        m_fov_target = m_perspective_camera->GetFOV();
    }

    void CPerspectiveCameraController::Update(float delta) {
        Math::SVec3 position = m_perspective_camera->GetPosition();
        Math::SVec3 forward = m_perspective_camera->GetForward();
        Math::SVec3 up = m_perspective_camera->GetUp();

        float fov = m_perspective_camera->GetFOV();

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

            float rotation_speed = 100.0f * delta;
            if (CInput::GetKey(EKeyCode::Left) || CInput::GetKey(EKeyCode::Q)) {
                m_yaw -= rotation_speed;
            }
            if (CInput::GetKey(EKeyCode::Right) || CInput::GetKey(EKeyCode::E)) {
                m_yaw += rotation_speed;
            }
            if (CInput::GetKey(EKeyCode::Up)) {
                m_pitch += rotation_speed;
            }
            if (CInput::GetKey(EKeyCode::Down)) {
                m_pitch -= rotation_speed;
            }

            m_yaw += x_offset;
            m_pitch += y_offset;
            m_pitch = CMathf::Clamp(m_pitch, -89.0f, 89.0f);

            SVec3 new_forward = SVec3::Zero();
            new_forward.x = CMathf::Cos(CMathf::ToRadians(m_pitch)) * CMathf::Cos(CMathf::ToRadians(m_yaw));
            new_forward.y = CMathf::Sin(CMathf::ToRadians(m_pitch));
            new_forward.z = CMathf::Cos(CMathf::ToRadians(m_pitch)) * CMathf::Sin(CMathf::ToRadians(m_yaw));
            forward = new_forward.Normalized();
        }

        // Zoom
        {
            float wheel = CInput::GetMouseScroll();
            m_fov_target -= wheel * 5.0f;
            m_fov_target = CMathf::Clamp(m_fov_target, 25, 120);
            fov = CMathf::Lerp(fov, m_fov_target, delta * 15);
        }

        // Movement
        {
            float camera_speed = m_speed * delta;
            SVec3 direction = camera_speed * forward;
            SVec3 right = SVec3::Cross(forward, up).Normalized();
            right = camera_speed * right;
            SVec3 up = camera_speed * SVec3::Up();
            if (CInput::GetKey(EKeyCode::W)) {
                position = position + direction;
            }
            if (CInput::GetKey(EKeyCode::S)) {
                position = position - direction;
            }
            if (CInput::GetKey(EKeyCode::A)) {
                position = position - right;
            }
            if (CInput::GetKey(EKeyCode::D)) {
                position = position + right;
            }
            if (CInput::GetKey(EKeyCode::LeftShift) || CInput::GetKey(EKeyCode::RightShift)) {
                position = position - up;
            }
            if (CInput::GetKey(EKeyCode::Space)) {
                position = position + up;
            }
        }

        // Reset
        if (CInput::GetKeyDown(EKeyCode::R)) {
            position = SVec3(0, 0, 1);
            forward = SVec3(0, 0, -1);
            up = SVec3::Up();

            m_pitch = 0.0f;
            m_yaw = -90.0f;

            fov = 90.0f;
            m_fov_target = fov;
        }

        // Set new properties
        m_perspective_camera->SetPosition(position);
        m_perspective_camera->SetForward(forward);
        m_perspective_camera->SetUp(up);

        m_perspective_camera->SetFOV(fov);

        m_perspective_camera->RecalculateMatricies();
    }

}