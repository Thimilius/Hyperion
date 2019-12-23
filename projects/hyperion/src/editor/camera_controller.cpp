#include "hyppch.hpp"

#include "hyperion/editor/camera_controller.hpp"

#include "hyperion/core/app/input.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion::Editor {

    void CameraController::Update(f32 delta) {
        Vec3 position = m_camera->GetPosition();
        Vec3 forward = m_camera->GetForward();
        Vec3 up = m_camera->GetUp();

        f32 fov = m_camera->GetFOV();
        f32 size = m_camera->GetSize();

        // Rotation
        {
            if (Input::GetMouseButtonDown(MouseButtonCode::Right) || Input::GetMouseButtonDown(MouseButtonCode::Middle)) {
                m_last_mouse_position = Input::GetMousePosition();
            }

            f32 x_offset = 0.0f;
            f32 y_offset = 0.0f;
            if (Input::GetMouseButton(MouseButtonCode::Right) || Input::GetMouseButton(MouseButtonCode::Middle)) {
                Vec2 mouse_position = Input::GetMousePosition();
                x_offset = mouse_position.x - m_last_mouse_position.x;
                y_offset = m_last_mouse_position.y - mouse_position.y;
                m_last_mouse_position.x = mouse_position.x;
                m_last_mouse_position.y = mouse_position.y;
            }

            f32 sensitivity = 0.25f;
            x_offset *= sensitivity;
            y_offset *= sensitivity;

            f32 rotation_speed = 100.0f * delta;
            if (Input::GetKey(KeyCode::Left) || Input::GetKey(KeyCode::Q)) {
                m_yaw -= rotation_speed;
            }
            if (Input::GetKey(KeyCode::Right) || Input::GetKey(KeyCode::E)) {
                m_yaw += rotation_speed;
            }
            if (Input::GetKey(KeyCode::Up)) {
                m_pitch += rotation_speed;
            }
            if (Input::GetKey(KeyCode::Down)) {
                m_pitch -= rotation_speed;
            }

            m_yaw += x_offset;
            m_pitch += y_offset;
            m_pitch = Math::Clamp(m_pitch, -89.0f, 89.0f);

            Vec3 new_forward = Vec3::Zero();
            new_forward.x = Math::Cos(Math::DegToRad(m_pitch)) * Math::Cos(Math::DegToRad(m_yaw));
            new_forward.y = Math::Sin(Math::DegToRad(m_pitch));
            new_forward.z = Math::Cos(Math::DegToRad(m_pitch)) * Math::Sin(Math::DegToRad(m_yaw));
            forward = new_forward.Normalized();
        }

        // Zoom
        {
            f32 wheel = Input::GetMouseScroll();
            switch (m_camera->GetType()) {
                case CameraType::Perspective: {
                    m_fov_target -= wheel * 5.0f;
                    m_fov_target = Math::Clamp(m_fov_target, 25, 120);
                    break;
                }
                case CameraType::Orthographic: {
                    m_size_target -= wheel * 0.25f;
                    m_size_target = Math::Clamp(m_size_target, 0.1f, 10);
                    break;
                }
                default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
            }
         
            fov = Math::Lerp(fov, m_fov_target, delta * 15);
            size = Math::Lerp(size, m_size_target, delta * 15);
        }

        // Movement
        {
            f32 camera_speed = m_speed * delta;
            Vec3 direction = camera_speed * forward;
            Vec3 right = Vec3::Cross(forward, up).Normalized();
            right = camera_speed * right;
            Vec3 up = camera_speed * Vec3::Up();
            if (Input::GetKey(KeyCode::W)) {
                position = position + direction;
            }
            if (Input::GetKey(KeyCode::S)) {
                position = position - direction;
            }
            if (Input::GetKey(KeyCode::A)) {
                position = position - right;
            }
            if (Input::GetKey(KeyCode::D)) {
                position = position + right;
            }
            if (Input::GetKey(KeyCode::LeftShift) || Input::GetKey(KeyCode::RightShift)) {
                position = position - up;
            }
            if (Input::GetKey(KeyCode::Space)) {
                position = position + up;
            }
        }

        // Reset
        if (Input::GetKeyDown(KeyCode::R)) {
            position = Vec3(2, 2, 2);
            forward = Vec3(-0.579227984f, -0.579227984f, -0.579227984f);
            up = Vec3::Up();

            m_pitch = -35.0f;
            m_yaw = 225.0f;

            fov = 90.0f;
            m_fov_target = fov;
            size = 2.75f;
            m_size_target = size;
        }

        // Set new properties
        m_camera->SetPosition(position);
        m_camera->SetForward(forward);
        m_camera->SetUp(up);
        m_camera->SetFOV(fov);
        m_camera->SetSize(size);
        m_camera->RecalculateMatricies();
    }

    Ref<CameraController> CameraController::Create(const Ref<Rendering::Camera> &camera) {
        return Ref<CameraController>(new CameraController(camera));
    }

    CameraController::CameraController(const Ref<Camera> &camera) {
        m_camera = camera;

        m_camera->SetPosition(Vec3(2, 2, 2));
        m_camera->SetForward(Vec3(0, 0, -1));
        m_camera->SetUp(Vec3(0, 1, 0));

        m_camera->SetFOV(90.0f);
        m_fov_target = m_camera->GetFOV();
        m_camera->SetSize(2.75f);
        m_size_target = m_camera->GetSize();

        m_camera->RecalculateMatricies();
    }

}