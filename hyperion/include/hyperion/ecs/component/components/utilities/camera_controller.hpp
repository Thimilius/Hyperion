//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/core/math/vector2.hpp>
#include <hyperion/core/math/vector3.hpp>
#include <hyperion/core/math/plane.hpp>
#include <hyperion/ecs/entity/entity_types.hpp>

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  struct DerivedTransformComponent;
  class EntityManager;

  namespace Rendering {
    struct CameraComponent;
  }}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  class CameraController {
  public:
    CameraController(EntityId camera) : m_camera(camera) { }
    virtual ~CameraController() = default;
  public:
    virtual void Reset(EntityManager *manager) = 0;
    virtual void Update(EntityManager *manager, float32 delta_time) = 0;
  protected:
    EntityId m_camera;
  };

  class FirstPersonCameraController final : public CameraController {
  public:
    FirstPersonCameraController(EntityId camera) : CameraController(camera) { }
  public:
    void Reset(EntityManager *manager) override;
    void Update(EntityManager *manager, float32 delta_time) override;
  private:
    Vector3 m_velocity;
    float32 m_acceleration = 100.0f;
    float32 m_friction = 10.0f;

    float32 m_pitch = -35.0f;
    float32 m_yaw = 45.0f;
    float32 m_target_pitch = m_pitch;
    float32 m_target_yaw = m_yaw;

    Vector2 m_last_mouse_position;

    float32 m_fov_target = 90.0f;
    float32 m_orthographic_size_target = 0;
  };

  class LookAroundCameraController final : public CameraController {
  public:
    LookAroundCameraController(EntityId camera) : CameraController(camera) { }
  public:
    void Reset(EntityManager *manager) override;
    void Update(EntityManager *manager, float32 delta_time) override;
  private:
    Vector3 GetPositionUnderMouse(Rendering::CameraComponent *camera, DerivedTransformComponent *derived_transform) const;
    Vector3 GetXZPlanePosition(Vector3 position, Vector3 forward) const;
    Vector3 GetLookAtPosition(Vector3 look_at_position, Vector3 position, Vector3 forward) const;
    float32 ClampAngle(float32 angle, float32 min, float32 max) const;
  private:
    Plane m_xz_plane = Plane(Vector3::Up(), 0);
    float32 m_xz_plane_distance = 1.0f;

    float32 m_rotation_axis_x = 0.0f;
    float32 m_rotation_axis_y = 0.0f;
    float32 m_rotation_velocity_x = 0.0f;
    float32 m_rotation_velocity_y = 0.0f;
    float32 m_zoom = 1.0f;

    Vector3 m_last_position;
    Vector2 m_last_mouse_position;

    float32 m_rotation_speed = 5.0f;
    float32 m_movement_speed = 0.001f;
    float32 m_zoom_speed = 0.05f;
  };

}
