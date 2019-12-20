#pragma once

#include "hyperion/rendering/camera.hpp"

namespace Hyperion::Entity {

    class CameraController {
    protected:
        f32 m_speed = 5.0f;
        f32 m_pitch = -35.0f;
        f32 m_yaw = 225.0f;

        Vec2 m_last_mouse_position;
    public:
        virtual void Update(f32 delta) = 0;
    };

    class PerspectiveCameraController : CameraController {
    private:
        Ref<Rendering::PerspectiveCamera> m_perspective_camera;

        f32 m_fov_target;
    public:
        PerspectiveCameraController(const Ref<Rendering::PerspectiveCamera> &perspective_camera);

        inline const Ref<Rendering::PerspectiveCamera> &GetCamera() const { return m_perspective_camera; }

        void Update(f32 delta) override;
    };

    class OrthographicCameraController : CameraController {
    private:
        Ref<Rendering::OrthographicCamera> m_orthographic_camera;

        f32 m_size_target;
    public:
        OrthographicCameraController(const Ref<Rendering::OrthographicCamera> &orthographic_camera);

        inline const Ref<Rendering::OrthographicCamera> &GetCamera() const { return m_orthographic_camera; }

        void Update(f32 delta) override;
    };

}