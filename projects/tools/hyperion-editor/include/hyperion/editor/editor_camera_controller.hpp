#pragma once

#include <hyperion/core/math/vec2.hpp>
#include <hyperion/entity/components/rendering/camera.hpp>

namespace Hyperion::Editor {

    class EditorCameraController {
    private:
        Camera *m_camera;

        Vec3 m_velocity;
        f32 m_acceleration = 100.0f;
        f32 m_friction = 10.0f;

        f32 m_pitch = -35.0f;
        f32 m_yaw = 45.0f;
        f32 m_target_pitch = m_pitch;
        f32 m_target_yaw = m_yaw;

        Vec2 m_last_mouse_position;

        f32 m_fov_target;
        f32 m_orthographic_size_target;
    public:
        EditorCameraController() = default;
        EditorCameraController(Camera *camera);

        inline const Camera *GetCamera() const { return m_camera; }

        void Update(f32 delta_time);
    };

}