//--------------------- Definition Include ---------------------
#include "hyperion/editor/world_view/editor_world_view_camera_controller.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/core/math/math.hpp>
#include <hyperion/core/app/input.hpp>
#include <hyperion/entity/entity.hpp>
#include <hyperion/entity/components/transform.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    //--------------------------------------------------------------
    void EditorCameraController::OnCreate() {
        m_camera = GetEntity()->GetComponent<Camera>();
        m_transform = m_camera->GetTransform();

        EnableUpdate();
    }

    //--------------------------------------------------------------
    Vec3 EditorFirstPersonCameraController::GetTargetPosition() const {
        return m_camera->GetTransform()->GetPosition();
    }

    //--------------------------------------------------------------
    void EditorFirstPersonCameraController::OnCreate() {
        EditorCameraController::OnCreate();

        GetEntity()->GetTransform()->SetPosition(Vec3(2.0f, 2.0f, 2.0f));
    }

    //--------------------------------------------------------------
    void EditorFirstPersonCameraController::OnUpdate(float32 delta_time) {
        Vec3 position = m_transform->GetPosition();
        Vec3 euler_angles = m_transform->GetEulerAngles();

        float32 fov = m_camera->GetFOV();
        float32 orthographic_size = m_camera->GetOrthographicSize();

        // Handle everything related to rotation.
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

            euler_angles = Vec3(m_pitch, m_yaw, 0);
        }

        // Handle everything related to zooming.
        {
            float32 wheel = Input::GetMouseScroll();
            switch (m_camera->GetProjectionMode()) {
                case Rendering::CameraProjectionMode::Perspective:
                {
                    m_fov_target -= wheel * 5.0f;
                    m_fov_target = Math::Clamp(m_fov_target, 25, 120);
                    break;
                }
                case Rendering::CameraProjectionMode::Orthographic:
                {
                    m_orthographic_size_target -= wheel * 0.25f;
                    m_orthographic_size_target = Math::Clamp(m_orthographic_size_target, 0.1f, 10);
                    break;
                }
                default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
            }

            fov = Math::Lerp(fov, m_fov_target, delta_time * 15);
            orthographic_size = Math::Lerp(orthographic_size, m_orthographic_size_target, delta_time * 15);
        }

        // Handle everything related to movement.
        {
            float32 acceleration = Input::IsKeyHold(KeyCode::Alt) ? m_acceleration * 5.0f : m_acceleration;
            float32 camera_acceleration = acceleration * delta_time;

            Vec3 forward = camera_acceleration * m_transform->GetForward();
            Vec3 right = camera_acceleration * m_transform->GetRight();
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

        m_transform->SetPosition(position);
        m_transform->SetEulerAngles(euler_angles);
        m_camera->SetFOV(fov);
        m_camera->SetOrthographicSize(orthographic_size);
    }

    //--------------------------------------------------------------
    void EditorFirstPersonCameraController::Reset() {
        Vec3 position = Vec3(2, 2, 2);

        m_velocity = Vec3::Zero();

        m_pitch = -35.0f;
        m_yaw = 45.0f;
        m_target_pitch = m_pitch;
        m_target_yaw = m_yaw;

        float32 fov = 90.0f;
        m_fov_target = fov;
        float32 orthographic_size = 2.75f;
        m_orthographic_size_target = orthographic_size;

        m_transform->SetPosition(position);
        m_camera->SetFOV(fov);
        m_camera->SetOrthographicSize(orthographic_size);
    }

    //--------------------------------------------------------------
    Vec3 EditorLookAroundCameraController::GetTargetPosition() const {
        return GetXZPlanePosition();
    }

    //--------------------------------------------------------------
    void EditorLookAroundCameraController::OnCreate() {
        EditorCameraController::OnCreate();

        m_camera->SetFOV(60);
        m_transform = GetEntity()->GetTransform();
        m_transform->SetEulerAngles(Vec3(-45, 45, 0));

        Vec3 angles = m_transform->GetEulerAngles();
        m_rotation_axis_x = angles.x;
        m_rotation_axis_y = angles.y;

        m_xz_plane_distance = 6;
        m_zoom = m_xz_plane_distance;
    }

    //--------------------------------------------------------------
    void EditorLookAroundCameraController::OnUpdate(float32 delta_time) {
        Vec2 current_mouse_position = Input::GetMousePosition();
        Vec2 mouse_position_difference = m_last_mouse_position - current_mouse_position;
        float32 mouse_axis_x = mouse_position_difference.x;
        float32 mouse_axis_y = mouse_position_difference.y;

        {
            if (Input::IsMouseButtonHold(MouseButtonCode::Middle)) {
                Vec3 position = m_transform->GetPosition();
                position += m_transform->GetRight() * mouse_axis_x * m_xz_plane_distance * m_movement_speed;
                position += (m_transform->GetUp() + m_transform->GetForward()).Normalized() * mouse_axis_y * m_xz_plane_distance * m_movement_speed;

                m_transform->SetPosition(position);
            }
        }

        {
            if (Input::IsMouseButtonHold(MouseButtonCode::Right)) {
                m_rotation_velocity_x += m_rotation_speed * mouse_axis_x * delta_time;
                m_rotation_velocity_y += m_rotation_speed * mouse_axis_y * delta_time;
            }

            m_rotation_axis_x -= m_rotation_velocity_y;
            m_rotation_axis_y += m_rotation_velocity_x;
            m_rotation_axis_x = ClampAngle(m_rotation_axis_x, -90, 90);
            Quaternion rotation = Quaternion::FromEulerAngles(m_rotation_axis_x, m_rotation_axis_y, 0);

            m_zoom -= Input::GetMouseScroll() * m_xz_plane_distance * m_zoom_speed;
            m_zoom = Math::Clamp(m_zoom, 0.05f, 1000.0f);
            m_xz_plane_distance = Math::Lerp(m_xz_plane_distance, m_zoom, 15.0f * delta_time);

            Vec3 plane_position = GetXZPlanePosition();
            Vec3 position = (rotation * Vec3(0, 0, m_xz_plane_distance)) + plane_position;
            m_transform->SetRotation(rotation);
            m_transform->SetPosition(position);

            m_rotation_velocity_x = Math::Lerp(m_rotation_velocity_x, 0.0f, 50.0f * delta_time);
            m_rotation_velocity_y = Math::Lerp(m_rotation_velocity_y, 0.0f, 50.0f * delta_time);
            m_last_mouse_position = current_mouse_position;
        }
    }

    //--------------------------------------------------------------
    void EditorLookAroundCameraController::Reset() {
        m_transform->SetEulerAngles(Vec3(-45, 45, 0));
        m_transform->SetPosition(GetLookAtPosition(Vec3::Zero()));

        Vec3 angles = m_transform->GetEulerAngles();
        m_rotation_axis_x = angles.x;
        m_rotation_axis_y = angles.y;

        m_xz_plane_distance = 6;
        m_zoom = m_xz_plane_distance;
    }

    //--------------------------------------------------------------
    Vec3 EditorLookAroundCameraController::GetPositionUnderMouse() const {
        Ray ray = m_camera->ScreenPointToRay(Input::GetMousePosition());
        float32 hit_distance = 0;
        m_xz_plane.Intersects(ray, hit_distance);
        return ray.GetPoint(hit_distance);
    }

    //--------------------------------------------------------------
    Vec3 EditorLookAroundCameraController::GetXZPlanePosition() const {
        Ray ray = Ray(m_transform->GetPosition(), m_transform->GetForward());
        float32 hit_distance = 0;
        m_xz_plane.Intersects(ray, hit_distance);
        return ray.GetPoint(hit_distance);
    }

    //--------------------------------------------------------------
    Vec3 EditorLookAroundCameraController::GetLookAtPosition(Vec3 position) const {
        Ray ray_from_camera = Ray(m_transform->GetPosition(), m_transform->GetForward());
        float32 hit_distance = 0;
        m_xz_plane.Intersects(ray_from_camera, hit_distance);
        Ray ray_to_camera = Ray(position, -m_transform->GetForward());
        return ray_to_camera.GetPoint(hit_distance);
    }

    //--------------------------------------------------------------
    float32 EditorLookAroundCameraController::ClampAngle(float32 angle, float32 min, float32 max) const {
        if (angle < -360.0f) {
            angle += 360.0f;
        }
        if (angle > 360.0f) {
            angle -= 360.0f;
        }
        return Math::Clamp(angle, min, max);
    }

    //--------------------------------------------------------------
    HYP_REFLECT_BEGIN(EditorCameraController)
    HYP_REFLECT_BASE(Component)
    HYP_REFLECT_END()

    //--------------------------------------------------------------
    HYP_REFLECT_BEGIN(EditorFirstPersonCameraController)
    HYP_REFLECT_BASE(EditorCameraController)
    HYP_REFLECT_CONSTRUCTOR([]() { return new EditorFirstPersonCameraController(); })
    HYP_REFLECT_END()

    //--------------------------------------------------------------
    HYP_REFLECT_BEGIN(EditorLookAroundCameraController)
    HYP_REFLECT_BASE(EditorCameraController)
    HYP_REFLECT_CONSTRUCTOR([]() { return new EditorLookAroundCameraController(); })
    HYP_REFLECT_END()

}