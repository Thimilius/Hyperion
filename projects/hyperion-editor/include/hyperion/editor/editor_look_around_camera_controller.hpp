#pragma once

#include <hyperion/core/math/vec2.hpp>
#include <hyperion/core/math/vec3.hpp>
#include <hyperion/core/math/plane.hpp>
#include <hyperion/entity/components/rendering/camera.hpp>

namespace Hyperion::Editor {

    class EditorLookAroundCameraController : public Component {
        HYP_REFLECT(Component);
    public:
        EditorLookAroundCameraController() = default;
    protected:
        void OnCreate() override;
        void OnUpdate(float32 delta_time) override;
    private:
        Vec3 GetPositionUnderMouse();
        Vec3 GetXZPlanePosition();
        Vec3 GetLookAtPosition(Vec3 position);
        float32 ClampAngle(float32 angle, float32 min, float32 max);
    private:
        Camera *m_camera;
        Transform *m_transform;

        Plane m_xz_plane = Plane(Vec3::Up(), 0);
        float32 m_xz_plane_distance = 1.0f;

        float32 m_rotation_axis_x;
        float32 m_rotation_axis_y;
        float32 m_rotation_velocity_x;
        float32 m_rotation_velocity_y;
        float32 m_zoom = 1.0f;

        Vec3 m_last_position;
        Vec2 m_last_mouse_position;

        float32 m_rotation_speed = 15.0f;
        float32 m_movement_speed = 1.0f;
        float32 m_zoom_speed = 75.0f;
    };

}
