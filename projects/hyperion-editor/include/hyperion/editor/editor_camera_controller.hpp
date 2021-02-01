#pragma once

#include <hyperion/core/math/vec2.hpp>
#include <hyperion/core/math/vec3.hpp>
#include <hyperion/entity/components/rendering/camera.hpp>

namespace Hyperion::Editor {

    class EditorCameraController : public Component {
        HYP_REFLECT(Component);
    public:
        EditorCameraController() = default;
    protected:
        void OnCreate() override;
        void OnUpdate(float32 delta_time) override;
    private:
        Camera *m_camera;

        Vec3 m_velocity;
        float32 m_acceleration = 100.0f;
        float32 m_friction = 10.0f;

        float32 m_pitch = -35.0f;
        float32 m_yaw = 45.0f;
        float32 m_target_pitch = m_pitch;
        float32 m_target_yaw = m_yaw;

        Vec2 m_last_mouse_position;

        float32 m_fov_target;
        float32 m_orthographic_size_target;
    };

}