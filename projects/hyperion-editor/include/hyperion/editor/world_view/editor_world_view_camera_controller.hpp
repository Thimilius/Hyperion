#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/core/math/vec2.hpp>
#include <hyperion/core/math/vec3.hpp>
#include <hyperion/core/math/plane.hpp>
#include <hyperion/entity/components/component.hpp>
#include <hyperion/entity/components/rendering/camera.hpp>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    class EditorCameraController : public Component {
        HYP_REFLECT(Component);
    public:
        virtual Vec3 GetTargetPosition() const = 0;

        virtual void Update(float32 delta_time) = 0;
        virtual void Reset() = 0;
    protected:
        void OnCreate() override;
    protected:
        Camera *m_camera;
        Transform *m_transform;
    };

    class EditorFirstPersonCameraController : public EditorCameraController {
        HYP_REFLECT(EditorCameraController);
    public:
        EditorFirstPersonCameraController() = default;
    protected:
        Vec3 GetTargetPosition() const override;

        void Update(float32 delta_time) override;
        void Reset() override;
    protected:
        void OnCreate() override;
    private:
        Vec3 m_velocity;
        float32 m_acceleration = 100.0f;
        float32 m_friction = 10.0f;

        float32 m_pitch = -35.0f;
        float32 m_yaw = 45.0f;
        float32 m_target_pitch = m_pitch;
        float32 m_target_yaw = m_yaw;

        Vec2 m_last_mouse_position;

        float32 m_fov_target = 90.0f;
        float32 m_orthographic_size_target;
    };

    class EditorLookAroundCameraController : public EditorCameraController {
        HYP_REFLECT(Component);
    public:
        EditorLookAroundCameraController() = default;
    public:
        Vec3 GetTargetPosition() const override;

        void Update(float32 delta_time) override;
        void Reset() override;
    protected:
        void OnCreate() override;
    private:
        Vec3 GetPositionUnderMouse() const;
        Vec3 GetXZPlanePosition() const;
        Vec3 GetLookAtPosition(Vec3 position) const;
        float32 ClampAngle(float32 angle, float32 min, float32 max) const;
    private:
        Plane m_xz_plane = Plane(Vec3::Up(), 0);
        float32 m_xz_plane_distance = 1.0f;

        float32 m_rotation_axis_x;
        float32 m_rotation_axis_y;
        float32 m_rotation_velocity_x;
        float32 m_rotation_velocity_y;
        float32 m_zoom = 1.0f;

        Vec3 m_last_position;
        Vec2 m_last_mouse_position;

        float32 m_rotation_speed = 5.0f;
        float32 m_movement_speed = 0.001f;
        float32 m_zoom_speed = 0.05f;
    };

}