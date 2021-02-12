#pragma once

//---------------------- Library Includes ----------------------
#include <hyperion/core/math/vec2.hpp>
#include <hyperion/core/math/vec3.hpp>
#include <hyperion/core/math/plane.hpp>

//---------------------- Project Includes ----------------------
#include "hyperion/editor/entity/components/editor_camera_controller.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Editor {

    class EditorLookAroundCameraController : public EditorCameraController {
        HYP_REFLECT(Component);
    public:
        EditorLookAroundCameraController() = default;
    protected:
        void OnCreate() override;
        void OnUpdate(float32 delta_time);
    private:
        Vec3 GetPositionUnderMouse();
        Vec3 GetXZPlanePosition();
        Vec3 GetLookAtPosition(Vec3 position);
        float32 ClampAngle(float32 angle, float32 min, float32 max);
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
