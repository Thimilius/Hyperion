#pragma once

#include "hyperion/rendering/camera.hpp"

namespace Hyperion::Entity {

    class CameraController {
    protected:
        float m_speed = 5.0f;
        float m_yaw = -90.0f;
        float m_pitch = 0.0f;

        Vec2 m_last_mouse_position;
    public:
        virtual void Update(float delta) = 0;
    };

    class PerspectiveCameraController : CameraController {
    private:
        Ref<Rendering::PerspectiveCamera> m_perspective_camera;

        float m_speed = 5.0f;
        float m_fov_target;
        float m_yaw = -90.0f;
        float m_pitch = 0.0f;

        Vec2 m_last_mouse_position;
    public:
        PerspectiveCameraController(const Ref<Rendering::PerspectiveCamera> &perspective_camera);

        inline const Ref<Rendering::PerspectiveCamera> &GetCamera() const { return m_perspective_camera; }

        void Update(float delta) override;
    };

    class OrthographicCameraController : CameraController {
    private:
        Ref<Rendering::OrthographicCamera> m_orthographic_camera;

        float m_speed = 5.0f;
        float m_size_target;
        float m_yaw = -90.0f;
        float m_pitch = 0.0f;

        Vec2 m_last_mouse_position;
    public:
        OrthographicCameraController(const Ref<Rendering::OrthographicCamera> &orthographic_camera);

        inline const Ref<Rendering::OrthographicCamera> &GetCamera() const { return m_orthographic_camera; }

        void Update(float delta) override;
    };

}