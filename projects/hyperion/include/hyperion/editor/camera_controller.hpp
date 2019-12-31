#pragma once

#include "hyperion/rendering/camera.hpp"

namespace Hyperion::Editor {

    class CameraController {
    private:
        Ref<Rendering::Camera> m_camera;

        Vec3 m_velocity;
        f32 m_acceleration = 100.0f;
        f32 m_friction = 10.0f;

        f32 m_pitch = -35.0f;
        f32 m_yaw = 225.0f;
        f32 m_target_pitch = m_pitch;
        f32 m_target_yaw = m_yaw;

        Vec2 m_last_mouse_position;

        f32 m_fov_target;
        f32 m_size_target;
    public:
        inline const Ref<Rendering::Camera> &GetCamera() const { return m_camera; }

        void Update(f32 delta);

        static Ref<CameraController> Create(const Ref<Rendering::Camera> &camera);
    private:
        CameraController(const Ref<Rendering::Camera> &camera);
    };

}