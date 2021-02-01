#include "hyperion/editor/editor_camera_controller.hpp"

#include <hyperion/core/math/math.hpp>
#include <hyperion/core/app/input.hpp>
#include <hyperion/entity/entity.hpp>
#include <hyperion/entity/components/transform.hpp>

namespace Hyperion::Editor {

    void EditorCameraController::OnCreate() {
        m_camera = GetEntity()->GetComponent<Camera>();
        GetEntity()->GetTransform()->SetPosition(Vec3(2.0f, 2.0f, 2.0f));

        RegisterForUpdate();
    }

    void EditorCameraController::OnUpdate(float32 delta_time) {
        Transform *transform = m_camera->GetTransform();
        Vec3 position = transform->GetPosition();
        Vec3 rotation = transform->GetEulerAngles();

        float32 fov = m_camera->GetFOV();
        float32 orthographic_size = m_camera->GetOrthographicSize();

        // Rotation
        {
            if (Input::IsMouseButtonDown(MouseButtonCode::Right) || Input::IsMouseButtonDown(MouseButtonCode::Middle)) {
                m_last_mouse_position = Input::GetMousePosition();
            }

            float32 x_offset = 0.0f;
            float32 y_offset = 0.0f;
            if (Input::IsMouseButtonHold(MouseButtonCode::Right) || Input::IsMouseButtonHold(MouseButtonCode::Middle)) {
                Vec2 mouse_position = Input::GetMousePosition();
                x_offset = mouse_position.x - m_last_mouse_position.x;
                y_offset = mouse_position.y - m_last_mouse_position.y;
                m_last_mouse_position.x = mouse_position.x;
                m_last_mouse_position.y = mouse_position.y;
            }

            float32 sensitivity = 0.2f;
            x_offset *= sensitivity;
            y_offset *= sensitivity;

            float32 rotation_speed = 100.0f * delta_time;
            if (Input::IsKeyHold(KeyCode::Left) || Input::IsKeyHold(KeyCode::Q)) {
                m_target_yaw += rotation_speed;
            }
            if (Input::IsKeyHold(KeyCode::Right) || Input::IsKeyHold(KeyCode::E)) {
                m_target_yaw -= rotation_speed;
            }
            if (Input::IsKeyHold(KeyCode::Up)) {
                m_target_pitch += rotation_speed;
            }
            if (Input::IsKeyHold(KeyCode::Down)) {
                m_target_pitch -= rotation_speed;
            }

            Vec2 right_stick = Input::GetGamepadAxis(Gamepad::Gamepad1, GamepadAxis::RightStick);
            m_target_yaw -= rotation_speed * right_stick.x;
            m_target_pitch += rotation_speed * right_stick.y;

            m_target_yaw -= x_offset;
            m_target_pitch += y_offset;
            m_target_pitch = Math::Clamp(m_target_pitch, -89.0f, 89.0f);

            float32 mouse_friction = 20.0f;
            m_yaw = Math::Lerp(m_yaw, m_target_yaw, delta_time * mouse_friction);
            m_pitch = Math::Lerp(m_pitch, m_target_pitch, delta_time * mouse_friction);

            rotation = Vec3(m_pitch, m_yaw, 0);
        }

        // Zoom
        {
            float32 wheel = Input::GetMouseScroll();
            switch (m_camera->GetProjectionMode()) {
                case Rendering::CameraProjectionMode::Perspective: {
                    m_fov_target -= wheel * 5.0f;
                    m_fov_target = Math::Clamp(m_fov_target, 25, 120);
                    break;
                }
                case Rendering::CameraProjectionMode::Orthographic: {
                    m_orthographic_size_target -= wheel * 0.25f;
                    m_orthographic_size_target = Math::Clamp(m_orthographic_size_target, 0.1f, 10);
                    break;
                }
                default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
            }

            fov = Math::Lerp(fov, m_fov_target, delta_time * 15);
            orthographic_size = Math::Lerp(orthographic_size, m_orthographic_size_target, delta_time * 15);
        }

        // Movement
        {
            float32 camera_acceleration = m_acceleration * delta_time;

            Vec3 forward = camera_acceleration * transform->GetForward();
            Vec3 right = camera_acceleration * transform->GetRight();
            Vec3 up = camera_acceleration * Vec3::Up();

            position += m_velocity * delta_time;

            if (Input::IsKeyHold(KeyCode::W)) {
                m_velocity += forward;
            }
            if (Input::IsKeyHold(KeyCode::S)) {
                m_velocity -= forward;
            }
            if (Input::IsKeyHold(KeyCode::A)) {
                m_velocity -= right;
            }
            if (Input::IsKeyHold(KeyCode::D)) {
                m_velocity += right;
            }
            if (Input::IsKeyHold(KeyCode::LeftShift) || Input::IsKeyHold(KeyCode::RightShift)) {
                m_velocity -= up;
            }
            if (Input::IsKeyHold(KeyCode::Space)) {
                m_velocity += up;
            }

            Vec2 left_stick = Input::GetGamepadAxis(Gamepad::Gamepad1, GamepadAxis::LeftStick);
            m_velocity += forward * left_stick.y;
            m_velocity += right * left_stick.x;
            m_velocity += up * Input::GetGamepadAxis(Gamepad::Gamepad1, GamepadAxis::RightTrigger).x;
            m_velocity -= up * Input::GetGamepadAxis(Gamepad::Gamepad1, GamepadAxis::LeftTrigger).x;

            m_velocity -= m_friction * delta_time * m_velocity;
        }

        // Reset
        if (Input::IsKeyDown(KeyCode::R) || Input::IsGamepadButtonDown(Gamepad::Gamepad1, GamepadButtonCode::RightThumb)) {
            position = Vec3(2, 2, 2);

            m_velocity = Vec3::Zero();

            m_pitch = -35.0f;
            m_yaw = 45.0f;
            m_target_pitch = m_pitch;
            m_target_yaw = m_yaw;

            fov = 90.0f;
            m_fov_target = fov;
            orthographic_size = 2.75f;
            m_orthographic_size_target = orthographic_size;
        }

        m_camera->GetTransform()->SetPosition(position);
        m_camera->GetTransform()->SetEulerAngles(rotation);
        m_camera->SetFOV(fov);
        m_camera->SetOrthographicSize(orthographic_size);
    }

}

HYP_REFLECT_REGISTER_BEGIN
{
    Registration<Editor::EditorCameraController>("EditorCameraController")
        .constructor()(DefaultConstructorPolicy);
}
HYP_REFLECT_REGISTER_END
