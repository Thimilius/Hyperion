//--------------------- Definition Include ---------------------
#include "sandbox/camera_controller.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/core/math/math.hpp>
#include <hyperion/core/app/input.hpp>
#include <hyperion/ecs/component/components/render_components.hpp>
#include <hyperion/ecs/component/components/transform_components.hpp>
#include <hyperion/ecs/component/components/utilities/camera_utilities.hpp>
#include <hyperion/ecs/component/components/utilities/transform_utilities.hpp>
#include <hyperion/ecs/world/world.hpp>

//------------------------- Namespaces -------------------------
using namespace Hyperion;
using namespace Hyperion::Rendering;

//-------------------- Definition Namespace --------------------
namespace Sandbox {

    //--------------------------------------------------------------
    void FirstPersonCameraController::Reset(World *world) {
        DerivedTransformComponent *derived_transform = world->GetComponent<DerivedTransformComponent>(m_camera);
        CameraComponent *camera = world->GetComponent<CameraComponent>(m_camera);

        Vector3 position = Vector3(2, 2, 2);

        m_velocity = Vector3::Zero();

        m_pitch = -35.0f;
        m_yaw = 45.0f;
        m_target_pitch = m_pitch;
        m_target_yaw = m_yaw;

        float32 fov = 90.0f;
        m_fov_target = fov;
        float32 orthographic_size = 2.75f;
        m_orthographic_size_target = orthographic_size;

        TransformUtilities::SetPosition(world, m_camera, position);

        camera->fov = fov;
        camera->orthographic_size = orthographic_size;
    }

    //--------------------------------------------------------------
    void FirstPersonCameraController::Update(World *world, float32 delta_time) {
        DerivedTransformComponent *derived_transform = world->GetComponent<DerivedTransformComponent>(m_camera);
        CameraComponent *camera = world->GetComponent<CameraComponent>(m_camera);

        Vector3 position = derived_transform->position;
        Vector3 euler_angles = derived_transform->rotation.ToEulerAngles();

        float32 fov = camera->fov;
        float32 orthographic_size = camera->orthographic_size;

        // Handle everything related to rotation.
        {
            if (Input::IsMouseButtonDown(MouseButtonCode::Right) || Input::IsMouseButtonDown(MouseButtonCode::Middle)) {
                m_last_mouse_position = Input::GetMousePosition().ToFloat();
            }

            float32 x_offset = 0.0f;
            float32 y_offset = 0.0f;
            if (Input::IsMouseButtonHold(MouseButtonCode::Right) || Input::IsMouseButtonHold(MouseButtonCode::Middle)) {
                Vector2 mouse_position = Input::GetMousePosition().ToFloat();
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

            Vector2 right_stick = Input::GetGamepadAxis(Gamepad::Gamepad1, GamepadAxis::RightStick);
            m_target_yaw -= rotation_speed * right_stick.x;
            m_target_pitch += rotation_speed * right_stick.y;

            m_target_yaw -= x_offset;
            m_target_pitch += y_offset;
            m_target_pitch = Math::Clamp(m_target_pitch, -89.0f, 89.0f);

            float32 mouse_friction = 20.0f;
            m_yaw = Math::Lerp(m_yaw, m_target_yaw, delta_time * mouse_friction);
            m_pitch = Math::Lerp(m_pitch, m_target_pitch, delta_time * mouse_friction);

            euler_angles = Vector3(m_pitch, m_yaw, 0);
        }

        // Handle everything related to zooming.
        {
            float32 wheel = Input::GetMouseScroll();
            switch (camera->projection_mode) {
                case CameraProjectionMode::Perspective: {
                    m_fov_target -= wheel * 5.0f;
                    m_fov_target = Math::Clamp(m_fov_target, 25, 120);
                    break;
                }
                case CameraProjectionMode::Orthographic: {
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

            Vector3 right = TransformUtilities::GetRight(derived_transform);
            Vector3 forward = TransformUtilities::GetForward(derived_transform);

            Vector3 velocity_forward = camera_acceleration * forward;
            Vector3 velocity_right = camera_acceleration * right;
            Vector3 velocity_up = camera_acceleration * Vector3::Up();

            position += m_velocity * delta_time;

            if (Input::IsKeyHold(KeyCode::W)) {
                m_velocity += velocity_forward;
            }
            if (Input::IsKeyHold(KeyCode::S)) {
                m_velocity -= velocity_forward;
            }
            if (Input::IsKeyHold(KeyCode::A)) {
                m_velocity -= velocity_right;
            }
            if (Input::IsKeyHold(KeyCode::D)) {
                m_velocity += velocity_right;
            }
            if (Input::IsKeyHold(KeyCode::LeftShift) || Input::IsKeyHold(KeyCode::RightShift)) {
                m_velocity -= velocity_up;
            }
            if (Input::IsKeyHold(KeyCode::Space)) {
                m_velocity += velocity_up;
            }

            Vector2 left_stick = Input::GetGamepadAxis(Gamepad::Gamepad1, GamepadAxis::LeftStick);
            m_velocity += velocity_forward * left_stick.y;
            m_velocity += velocity_right * left_stick.x;
            m_velocity += velocity_up * Input::GetGamepadAxis(Gamepad::Gamepad1, GamepadAxis::RightTrigger).x;
            m_velocity -= velocity_up * Input::GetGamepadAxis(Gamepad::Gamepad1, GamepadAxis::LeftTrigger).x;

            m_velocity -= m_friction * delta_time * m_velocity;
        }

        TransformUtilities::SetPosition(world, m_camera, position);
        TransformUtilities::SetRotation(world, m_camera, Quaternion::FromEulerAngles(euler_angles));
        camera->fov = fov;
        camera->orthographic_size = orthographic_size;
    }

    //--------------------------------------------------------------
    void LookAroundCameraController::Update(World *world, float32 delta_time) {
        DerivedTransformComponent *derived_transform = world->GetComponent<DerivedTransformComponent>(m_camera);
        CameraComponent *camera = world->GetComponent<CameraComponent>(m_camera);

        Vector3 right = TransformUtilities::GetRight(derived_transform);
        Vector3 up = TransformUtilities::GetUp(derived_transform);
        Vector3 forward = TransformUtilities::GetForward(derived_transform);

        Vector2 current_mouse_position = Input::GetMousePosition().ToFloat();
        Vector2 mouse_position_difference = m_last_mouse_position - current_mouse_position;
        float32 mouse_axis_x = mouse_position_difference.x;
        float32 mouse_axis_y = mouse_position_difference.y;


        Vector3 position = derived_transform->position;
        {
            if (Input::IsMouseButtonHold(MouseButtonCode::Middle)) {
                position += right * mouse_axis_x * m_xz_plane_distance * m_movement_speed;
                position += (up + forward).Normalized() * mouse_axis_y * m_xz_plane_distance * m_movement_speed;

                TransformUtilities::SetPosition(world, m_camera, position);
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

            Vector3 plane_position = GetXZPlanePosition(position, forward);
            Vector3 position = (rotation * Vector3(0, 0, m_xz_plane_distance)) + plane_position;

            TransformUtilities::SetPosition(world, m_camera, position);
            TransformUtilities::SetRotation(world, m_camera, rotation);

            m_rotation_velocity_x = Math::Lerp(m_rotation_velocity_x, 0.0f, 50.0f * delta_time);
            m_rotation_velocity_y = Math::Lerp(m_rotation_velocity_y, 0.0f, 50.0f * delta_time);
            m_last_mouse_position = current_mouse_position;
        }
    }

    //--------------------------------------------------------------
    void LookAroundCameraController::Reset(World *world) {
        DerivedTransformComponent *derived_transform = world->GetComponent<DerivedTransformComponent>(m_camera);
        CameraComponent *camera = world->GetComponent<CameraComponent>(m_camera);

        Vector3 euler_angles = Vector3(-45, 45, 0);

        // We have to set the rotation first, so that the conversion to world position is correct.
        Quaternion rotation = Quaternion::FromEulerAngles(euler_angles);
        TransformUtilities::SetRotation(world, m_camera, rotation);
        Vector3 position = GetLookAtPosition(Vector3::Zero(), derived_transform->position, TransformUtilities::GetForward(derived_transform));
        TransformUtilities::SetPosition(world, m_camera, position);

        m_rotation_axis_x = euler_angles.x;
        m_rotation_axis_y = euler_angles.y;

        m_xz_plane_distance = 6;
        m_zoom = m_xz_plane_distance;
    }

    //--------------------------------------------------------------
    Vector3 LookAroundCameraController::GetPositionUnderMouse(CameraComponent *camera, DerivedTransformComponent *derived_transform) const {
        CameraUtilities::RecalculateMatricies(camera, derived_transform);
        Ray ray = CameraUtilities::ScreenPointToRay(camera, derived_transform, Input::GetMousePosition().ToFloat());
        float32 hit_distance = 0;
        m_xz_plane.Intersects(ray, hit_distance);
        return ray.GetPoint(hit_distance);
    }

    //--------------------------------------------------------------
    Vector3 LookAroundCameraController::GetXZPlanePosition(Vector3 position, Vector3 forward) const {
        Ray ray = Ray(position, forward);
        float32 hit_distance = 0;
        m_xz_plane.Intersects(ray, hit_distance);
        return ray.GetPoint(hit_distance);
    }

    //--------------------------------------------------------------
    Vector3 LookAroundCameraController::GetLookAtPosition(Vector3 look_at_position, Vector3 position, Vector3 forward) const {
        Ray ray_from_camera = Ray(position, forward);
        float32 hit_distance = 0;
        m_xz_plane.Intersects(ray_from_camera, hit_distance);
        Ray ray_to_camera = Ray(look_at_position, -forward);
        return ray_to_camera.GetPoint(hit_distance);
    }

    //--------------------------------------------------------------
    float32 LookAroundCameraController::ClampAngle(float32 angle, float32 min, float32 max) const {
        if (angle < -360.0f) {
            angle += 360.0f;
        }
        if (angle > 360.0f) {
            angle -= 360.0f;
        }
        return Math::Clamp(angle, min, max);
    }

}