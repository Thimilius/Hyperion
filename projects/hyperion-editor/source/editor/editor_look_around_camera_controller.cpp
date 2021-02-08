#include "hyperion/editor/editor_look_around_camera_controller.hpp"

#include <hyperion/core/math/math.hpp>
#include <hyperion/core/math/plane.hpp>
#include <hyperion/core/app/input.hpp>
#include <hyperion/entity/entity.hpp>
#include <hyperion/entity/components/transform.hpp>

namespace Hyperion::Editor {

    void EditorLookAroundCameraController::OnCreate() {
        m_camera = GetEntity()->GetComponent<Camera>();
        m_camera->SetFOV(60);
        m_transform = GetEntity()->GetTransform();
        m_transform->SetEulerAngles(Vec3(-45, 45, 0));

        Vec3 angles = m_transform->GetEulerAngles();
        m_rotation_axis_x = angles.x;
        m_rotation_axis_y = angles.y;

        m_xz_plane_distance = 6;
        m_zoom = m_xz_plane_distance;

        RegisterForUpdate();
    }

    void EditorLookAroundCameraController::OnUpdate(float32 delta_time) {
        {
            Vec3 current_position = GetPositionUnderMouse();
            Vec3 difference = m_last_position - current_position;
            if (Input::IsMouseButtonHold(MouseButtonCode::Middle)) {
                m_transform->SetPosition(m_transform->GetPosition() + difference);
                current_position = GetPositionUnderMouse();
            }
            m_last_position = current_position;
        }
        
        {
            Vec2 current_mouse_position = Input::GetMousePosition();
            Vec2 mouse_position_difference = m_last_mouse_position - current_mouse_position;
            float32 mouse_axis_x = mouse_position_difference.x;
            float32 mouse_axis_y = mouse_position_difference.y;

            if (Input::IsMouseButtonHold(MouseButtonCode::Right)) {
                m_rotation_velocity_x += m_rotation_speed * mouse_axis_x * delta_time;
                m_rotation_velocity_y += m_rotation_speed * mouse_axis_y * delta_time;
            }

            m_rotation_axis_x -= m_rotation_velocity_y;
            m_rotation_axis_y += m_rotation_velocity_x;
            m_rotation_axis_x = ClampAngle(m_rotation_axis_x, -90, 90);
            Quaternion rotation = Quaternion::FromEulerAngles(m_rotation_axis_x, m_rotation_axis_y, 0);

            m_zoom -= Input::GetMouseScroll() * m_xz_plane_distance * m_zoom_speed * delta_time;
            m_zoom = Math::Clamp(m_zoom, 0.05f, 1000.0f);
            m_xz_plane_distance = Math::Lerp(m_xz_plane_distance, m_zoom, 10.0f * delta_time);

            Vec3 plane_position = GetXZPlanePosition();
            Vec3 position = (rotation * Vec3(0, 0, m_xz_plane_distance)) + plane_position;
            m_transform->SetRotation(rotation);
            m_transform->SetPosition(position);

            m_rotation_velocity_x = Math::Lerp(m_rotation_velocity_x, 0.0f, 75.0f * delta_time);
            m_rotation_velocity_y = Math::Lerp(m_rotation_velocity_y, 0.0f, 75.0f * delta_time);
            m_last_mouse_position = current_mouse_position;
        }

        {
            if (Input::IsKeyDown(KeyCode::R)) {
                m_transform->SetEulerAngles(Vec3(-45, 45, 0));
                m_transform->SetPosition(GetLookAtPosition(Vec3::Zero()));

                Vec3 angles = m_transform->GetEulerAngles();
                m_rotation_axis_x = angles.x;
                m_rotation_axis_y = angles.y;

                m_xz_plane_distance = 6;
                m_zoom = m_xz_plane_distance;
            }
        }
    }

    Vec3 EditorLookAroundCameraController::GetPositionUnderMouse() {
        Ray ray = m_camera->ScreenPointToRay(Input::GetMousePosition());
        float32 hit_distance = 0;
        m_xz_plane.Intersects(ray, hit_distance);
        return ray.GetPoint(hit_distance);
    }

    Vec3 EditorLookAroundCameraController::GetXZPlanePosition() {
        Ray ray = Ray(m_transform->GetPosition(), m_transform->GetForward());
        float32 hit_distance = 0;
        m_xz_plane.Intersects(ray, hit_distance);
        return ray.GetPoint(hit_distance);
    }

    Vec3 EditorLookAroundCameraController::GetLookAtPosition(Vec3 position) {
        Ray ray_from_camera = Ray(m_transform->GetPosition(), m_transform->GetForward());
        float32 hit_distance = 0;
        m_xz_plane.Intersects(ray_from_camera, hit_distance);
        Ray ray_to_camera = Ray(position, -m_transform->GetForward());
        return ray_to_camera.GetPoint(hit_distance);
    }

    float32 EditorLookAroundCameraController::ClampAngle(float32 angle, float32 min, float32 max) {
        if (angle < -360.0f) {
            angle += 360.0f;
        }
        if (angle > 360.0f) {
            angle -= 360.0f;
        }
        return Math::Clamp(angle, min, max);
    }

}

HYP_REFLECT_REGISTER_BEGIN
{
    Registration<Editor::EditorLookAroundCameraController>("EditorLookAroundCameraController")
        .constructor()(DefaultConstructorPolicy);
}
HYP_REFLECT_REGISTER_END

