#include "hyppch.hpp"

#include "hyperion/editor/editor_camera_controller.hpp"

#include "hyperion/core/app/input.hpp"
#include "hyperion/entity/components/transform.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion::Editor {

    void EditorCameraController::Update(f32 delta_time) {
        Transform *transform = m_camera->GetTransform();
        Vec3 position = transform->GetPosition();
        Vec3 rotation = transform->GetEulerAngles();

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

            f32 sensitivity = 0.2f;
            x_offset *= sensitivity;
            y_offset *= sensitivity;

            f32 rotation_speed = 100.0f * delta_time;
            if (Input::GetKey(KeyCode::Left) || Input::GetKey(KeyCode::Q)) {
                m_target_yaw += rotation_speed;
            }
            if (Input::GetKey(KeyCode::Right) || Input::GetKey(KeyCode::E)) {
                m_target_yaw -= rotation_speed;
            }
            if (Input::GetKey(KeyCode::Up)) {
                m_target_pitch += rotation_speed;
            }
            if (Input::GetKey(KeyCode::Down)) {
                m_target_pitch -= rotation_speed;
            }

            Vec2 right_stick = Input::GetGamepadAxis(Gamepad::Gamepad1, GamepadAxis::RightStick);
            m_target_yaw -= rotation_speed * right_stick.x;
            m_target_pitch += rotation_speed * -right_stick.y;

            m_target_yaw -= x_offset;
            m_target_pitch += y_offset;
            m_target_pitch = Math::Clamp(m_target_pitch, -89.0f, 89.0f);

            f32 mouse_friction = 20.0f;
            m_yaw = Math::Lerp(m_yaw, m_target_yaw, delta_time * mouse_friction);
            m_pitch = Math::Lerp(m_pitch, m_target_pitch, delta_time * mouse_friction);

            rotation = Vec3(m_pitch, m_yaw, 0);
        }

        // Zoom
        {
            f32 wheel = Input::GetMouseScroll();
            switch (m_camera->GetMode()) {
                case CameraMode::Perspective: {
                    m_fov_target -= wheel * 5.0f;
                    m_fov_target = Math::Clamp(m_fov_target, 25, 120);
                    break;
                }
                case CameraMode::Orthographic: {
                    m_size_target -= wheel * 0.25f;
                    m_size_target = Math::Clamp(m_size_target, 0.1f, 10);
                    break;
                }
                default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
            }
         
            fov = Math::Lerp(fov, m_fov_target, delta_time * 15);
            size = Math::Lerp(size, m_size_target, delta_time * 15);
        }

        // Movement
        {
            f32 camera_acceleration = m_acceleration * delta_time;

            Vec3 direction = camera_acceleration * transform->GetForward();
            Vec3 right = camera_acceleration * Vec3::Cross(direction, transform->GetUp()).Normalized();
            Vec3 up = camera_acceleration * Vec3::Up();
            
            position += m_velocity * delta_time;

            if (Input::GetKey(KeyCode::W)) {
                m_velocity += direction;
            }
            if (Input::GetKey(KeyCode::S)) {
                m_velocity -= direction;
            }
            if (Input::GetKey(KeyCode::A)) {
                m_velocity -= right;
            }
            if (Input::GetKey(KeyCode::D)) {
                m_velocity += right;
            }
            if (Input::GetKey(KeyCode::LeftShift) || Input::GetKey(KeyCode::RightShift)) {
                m_velocity -= up;
            }
            if (Input::GetKey(KeyCode::Space)) {
                m_velocity += up;
            }

            Vec2 left_stick = Input::GetGamepadAxis(Gamepad::Gamepad1, GamepadAxis::LeftStick);
            m_velocity += direction * -left_stick.y;
            m_velocity += right * left_stick.x;
            m_velocity += up * Input::GetGamepadAxis(Gamepad::Gamepad1, GamepadAxis::RightTrigger).x;
            m_velocity -= up * Input::GetGamepadAxis(Gamepad::Gamepad1, GamepadAxis::LeftTrigger).x;

            m_velocity -= m_friction * delta_time * m_velocity;
        }

        // Reset
        if (Input::GetKeyDown(KeyCode::R) || Input::GetGamepadButtonDown(Gamepad::Gamepad1, GamepadButtonCode::RightThumb)) {
            position = Vec3(2, 2, 2);

            m_velocity = Vec3::Zero();

            m_pitch = -35.0f;
            m_yaw = 45.0f;
            m_target_pitch = m_pitch;
            m_target_yaw = m_yaw;

            fov = 90.0f;
            m_fov_target = fov;
            size = 2.75f;
            m_size_target = size;
        }

        m_camera->GetTransform()->SetPosition(position);
        m_camera->GetTransform()->SetEulerAngles(rotation);
        m_camera->SetFOV(fov);
        m_camera->SetSize(size);
    }

    EditorCameraController::EditorCameraController(Camera *camera) {
        m_camera = camera;

        m_camera->GetTransform()->SetPosition(Vec3(2, 2, 2));
        m_camera->GetTransform()->SetEulerAngles(Vec3::Zero());

        m_camera->SetFOV(90.0f);
        m_fov_target = m_camera->GetFOV();
        m_camera->SetSize(2.75f);
        m_size_target = m_camera->GetSize();
    }

}