//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/core/math/plane.hpp>
#include <hyperion/ecs/component/components/transform_components.hpp>
#include <hyperion/ecs/component/components/render_components.hpp>
#include <hyperion/render/frame/context/render_frame_context_camera.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

  class EditorCamera final {
  public:
    static void Initialize();
    static void Update(float32 delta_time);
    static void Reset();

    static Rendering::RenderFrameContextCamera GetContextCamera();
  private:
    static Vector3 GetPositionUnderMouse();
    static Vector3 GetXZPlanePosition(Vector3 position, Vector3 forward);
    static Vector3 GetLookAtPosition(Vector3 look_at_position, Vector3 position, Vector3 forward);
    static float32 ClampAngle(float32 angle, float32 min, float32 max);
  private:
    EditorCamera() = delete;
    ~EditorCamera() = delete;
  private:
    inline static Rendering::CameraComponent s_camera;
    inline static DerivedTransformComponent s_transform;

    inline static Plane m_xz_plane = Plane(Vector3::Up(), 0);
    inline static float32 m_xz_plane_distance = 1.0f;

    inline static float32 m_rotation_axis_x = 0.0f;
    inline static float32 m_rotation_axis_y = 0.0f;
    inline static float32 m_rotation_velocity_x = 0.0f;
    inline static float32 m_rotation_velocity_y = 0.0f;
    inline static float32 m_zoom = 1.0f;

    inline static Vector3 m_last_position;
    inline static Vector2 m_last_mouse_position;

    inline static float32 m_rotation_speed = 5.0f;
    inline static float32 m_movement_speed = 0.001f;
    inline static float32 m_zoom_speed = 0.05f;
  };

}