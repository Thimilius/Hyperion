//--------------------- Definition Include ---------------------
#include "hyperion/editor/editor_camera.hpp"

//---------------------- Library Includes ----------------------
#include <hyperion/core/app/input.hpp>
#include <hyperion/core/math/math.hpp>
#include <hyperion/ecs/component/components/render_components.hpp>
#include <hyperion/ecs/component/components/utilities/camera_utilities.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/editor_style.hpp"
#include "hyperion/editor/editor_ui.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

  //--------------------------------------------------------------
  void EditorCamera::Initialize() {
    Reset();
  }

  //--------------------------------------------------------------
  void EditorCamera::Update(float32 delta_time) {
    Vector3 right = s_transform.rotation * Vector3::Right();
    Vector3 up = s_transform.rotation * Vector3::Up();
    Vector3 forward = s_transform.rotation * Vector3::Forward();

    Vector2 current_mouse_position = Input::GetMousePosition().ToFloat();
    Vector2 mouse_position_difference = m_last_mouse_position - current_mouse_position;
    float32 mouse_axis_x = mouse_position_difference.x;
    float32 mouse_axis_y = mouse_position_difference.y;

    if (Input::IsMouseButtonUp(MouseButtonCode::Right) || Input::IsMouseButtonUp(MouseButtonCode::Middle)) {
      m_mouse_captured = false;
    }
    if (EditorUI::IsMouseInsidePreviewRect()) {
      m_should_receive_input = true;
      if (Input::IsMouseButtonDown(MouseButtonCode::Right) || Input::IsMouseButtonDown(MouseButtonCode::Middle)) {
        m_mouse_captured = true;
      }
    } else {
      m_should_receive_input = m_mouse_captured;  
    }
    
    Vector3 position = s_transform.position;
    {
      if (m_should_receive_input && Input::IsMouseButtonHold(MouseButtonCode::Middle)) {
        position += right * mouse_axis_x * m_xz_plane_distance * m_movement_speed;
        position += (up + forward).Normalized() * mouse_axis_y * m_xz_plane_distance * m_movement_speed;

        s_transform.position = position;
      }
    }

    {
      if (m_should_receive_input && Input::IsMouseButtonHold(MouseButtonCode::Right)) {
        m_rotation_velocity_x += m_rotation_speed * mouse_axis_x * delta_time;
        m_rotation_velocity_y += m_rotation_speed * mouse_axis_y * delta_time;
      }

      m_rotation_axis_x -= m_rotation_velocity_y;
      m_rotation_axis_y += m_rotation_velocity_x;
      m_rotation_axis_x = ClampAngle(m_rotation_axis_x, -90, 90);
      Quaternion rotation = Quaternion::FromEulerAngles(m_rotation_axis_x, m_rotation_axis_y, 0);

      if (m_should_receive_input) {
        m_zoom -= Input::GetMouseScroll() * m_xz_plane_distance * m_zoom_speed;  
      }
      m_zoom = Math::Clamp(m_zoom, 0.05f, 1000.0f);
      m_xz_plane_distance = Math::Lerp(m_xz_plane_distance, m_zoom, 15.0f * delta_time);

      Vector3 plane_position = GetXZPlanePosition(position, forward);
      Vector3 new_position = (rotation * Vector3(0, 0, m_xz_plane_distance)) + plane_position;

      s_transform.position = new_position;
      s_transform.rotation = rotation;

      m_rotation_velocity_x = Math::Lerp(m_rotation_velocity_x, 0.0f, 50.0f * delta_time);
      m_rotation_velocity_y = Math::Lerp(m_rotation_velocity_y, 0.0f, 50.0f * delta_time);
      m_last_mouse_position = current_mouse_position;
    }

    if (Input::IsKeyDown(KeyCode::R)) {
      Reset();
    }
  }

  //--------------------------------------------------------------
  void EditorCamera::Reset() {
    Vector3 euler_angles = Vector3(-45, 45, 0);

    // We have to set the rotation first, so that the conversion to world position is correct.
    Quaternion rotation = Quaternion::FromEulerAngles(euler_angles);
    s_transform.rotation = rotation;
    Vector3 position = GetLookAtPosition(Vector3::Zero(), s_transform.position, rotation * Vector3::Forward());
    s_transform.position = position;

    m_rotation_axis_x = euler_angles.x;
    m_rotation_axis_y = euler_angles.y;

    m_xz_plane_distance = 6;
    m_zoom = m_xz_plane_distance;
  }

  //--------------------------------------------------------------
  Rendering::RenderFrameContextCamera EditorCamera::GetContextCamera() {
    Rect preview_rect = EditorUI::GetPreviewRect();
    Rendering::CameraViewport viewport = { 0, 0, static_cast<uint32>(preview_rect.width), static_cast<uint32>(preview_rect.height) };
    
    Rendering::CameraUtilities::RecalculateMatrices(&s_camera, &s_transform, viewport);

    Rendering::RenderFrameContextCamera render_frame_context_camera;
    render_frame_context_camera.projection_mode = s_camera.projection_mode;
    render_frame_context_camera.clear_mode = s_camera.clear_mode;
    render_frame_context_camera.background_color = s_camera.background_color;
    render_frame_context_camera.culling_mask = s_camera.culling_mask;
    render_frame_context_camera.position = s_transform.position;
    render_frame_context_camera.forward = s_transform.rotation * Vector3::Forward();
    render_frame_context_camera.up = s_transform.rotation * Vector3::Up();
    render_frame_context_camera.fov = s_camera.fov;
    render_frame_context_camera.orthographic_size = s_camera.orthographic_size;
    render_frame_context_camera.near_plane = s_camera.near_plane;
    render_frame_context_camera.far_plane = s_camera.far_plane;
    render_frame_context_camera.view_matrix = s_camera.view_matrix;
    render_frame_context_camera.inverse_view_matrix = s_camera.view_matrix.Inverted();
    render_frame_context_camera.projection_matrix = s_camera.projection_matrix;
    render_frame_context_camera.inverse_projection_matrix = s_camera.projection_matrix.Inverted();
    render_frame_context_camera.view_projection_matrix = s_camera.view_projection_matrix;
    render_frame_context_camera.inverse_view_projection_matrix = s_camera.view_projection_matrix.Inverted();
    render_frame_context_camera.viewport = viewport;

    return render_frame_context_camera;
  }

  //--------------------------------------------------------------
  Vector3 EditorCamera::GetPositionUnderMouse() {
    Rendering::CameraUtilities::RecalculateMatrices(&s_camera, &s_transform);
    Ray ray = Rendering::CameraUtilities::ScreenPointToRay(&s_camera, &s_transform, Input::GetMousePosition().ToFloat());
    float32 hit_distance = 0;
    m_xz_plane.Intersects(ray, hit_distance);
    return ray.GetPoint(hit_distance);
  }

  //--------------------------------------------------------------
  Vector3 EditorCamera::GetXZPlanePosition(Vector3 position, Vector3 forward) {
    Ray ray = Ray(position, forward);
    float32 hit_distance = 0;
    m_xz_plane.Intersects(ray, hit_distance);
    return ray.GetPoint(hit_distance);
  }

  //--------------------------------------------------------------
  Vector3 EditorCamera::GetLookAtPosition(Vector3 look_at_position, Vector3 position, Vector3 forward) {
    Ray ray_from_camera = Ray(position, forward);
    float32 hit_distance = 0;
    m_xz_plane.Intersects(ray_from_camera, hit_distance);
    Ray ray_to_camera = Ray(look_at_position, -forward);
    return ray_to_camera.GetPoint(hit_distance);
  }

  //--------------------------------------------------------------
  float32 EditorCamera::ClampAngle(float32 angle, float32 min, float32 max) {
    if (angle < -360.0f) {
      angle += 360.0f;
    }
    if (angle > 360.0f) {
      angle -= 360.0f;
    }
    return Math::Clamp(angle, min, max);
  }

}
