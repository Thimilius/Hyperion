#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/core/math/vector2.hpp>
#include <hyperion/core/math/vector3.hpp>
#include <hyperion/core/math/plane.hpp>
#include <hyperion/ecs/entity/entity_types.hpp>

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    struct CameraComponent;
    struct LocalTransformComponent;
    class World;
}

//-------------------- Definition Namespace --------------------
namespace Sandbox {

    class CameraController {
    public:
        CameraController(Hyperion::EntityId camera) : m_camera(camera) { }
    public:
        virtual void Reset(Hyperion::World *world) = 0;
        virtual void Update(Hyperion::World *world, float32 delta_time) = 0;
    protected:
        Hyperion::EntityId m_camera;
    };

    class FirstPersonCameraController : public CameraController {
    public:
        FirstPersonCameraController(Hyperion::EntityId camera) : CameraController(camera) { }
    public:
        void Reset(Hyperion::World *world) override;
        void Update(Hyperion::World *world, float32 delta_time) override;
    private:
        Hyperion::Vector3 m_velocity;
        float32 m_acceleration = 100.0f;
        float32 m_friction = 10.0f;

        float32 m_pitch = -35.0f;
        float32 m_yaw = 45.0f;
        float32 m_target_pitch = m_pitch;
        float32 m_target_yaw = m_yaw;

        Hyperion::Vector2 m_last_mouse_position;

        float32 m_fov_target = 90.0f;
        float32 m_orthographic_size_target;
    };

    class LookAroundCameraController : public CameraController {
    public:
        LookAroundCameraController(Hyperion::EntityId camera) : CameraController(camera) { }
    public:
        void Reset(Hyperion::World *world) override;
        void Update(Hyperion::World *world, float32 delta_time) override;
    private:
        Hyperion::Vector3 GetPositionUnderMouse(Hyperion::CameraComponent *camera, Hyperion::LocalTransformComponent *local_transform) const;
        Hyperion::Vector3 GetXZPlanePosition(Hyperion::Vector3 position, Hyperion::Vector3 forward) const;
        Hyperion::Vector3 GetLookAtPosition(Hyperion::Vector3 look_at_position, Hyperion::Vector3 position, Hyperion::Vector3 forward) const;
        float32 ClampAngle(float32 angle, float32 min, float32 max) const;
    private:
        Hyperion::Plane m_xz_plane = Hyperion::Plane(Hyperion::Vector3::Up(), 0);
        float32 m_xz_plane_distance = 1.0f;

        float32 m_rotation_axis_x;
        float32 m_rotation_axis_y;
        float32 m_rotation_velocity_x;
        float32 m_rotation_velocity_y;
        float32 m_zoom = 1.0f;

        Hyperion::Vector3 m_last_position;
        Hyperion::Vector2 m_last_mouse_position;

        float32 m_rotation_speed = 5.0f;
        float32 m_movement_speed = 0.001f;
        float32 m_zoom_speed = 0.05f;
    };

}