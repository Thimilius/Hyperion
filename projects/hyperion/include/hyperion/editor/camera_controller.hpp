#pragma once

#include "hyperion/rendering/camera.hpp"

namespace Hyperion::Editor {

    class CameraController {
    private:
        Ref<Rendering::Camera> m_camera;

        f32 m_speed = 5.0f;
        f32 m_pitch = -35.0f;
        f32 m_yaw = 225.0f;

        Vec2 m_last_mouse_position;

        f32 m_fov_target;
        f32 m_size_target;
    public:
        CameraController(const Ref<Rendering::Camera> &camera);

        inline const Ref<Rendering::Camera> &GetCamera() const { return m_camera; }

        void Update(f32 delta);
    };

}