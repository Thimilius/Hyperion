#pragma once

#include "hyperion/rendering/camera.hpp"

namespace Hyperion::Entity {

    class CPerspectiveCameraController {
    private:
        TRef<Rendering::CPerspectiveCamera> m_perspective_camera;

        float m_speed = 5.0f;
        float m_fov_target;
        float m_yaw = -90.0f;
        float m_pitch = 0.0f;

        Math::SVec2 m_last_mouse_position;
    public:
        CPerspectiveCameraController(const TRef<Rendering::CPerspectiveCamera> &perspective_camera);

        inline const TRef<Rendering::CPerspectiveCamera> &GetCamera() const { return m_perspective_camera; }

        void Update(float delta);
    };

    class COrthographicCameraController {
    private:
        TRef<Rendering::COrthographicCamera> m_orthographic_camera;

        float m_speed = 5.0f;
        float m_size_target;
        float m_yaw = -90.0f;
        float m_pitch = 0.0f;

        Math::SVec2 m_last_mouse_position;
    public:
        COrthographicCameraController(const TRef<Rendering::COrthographicCamera> &orthographic_camera);

        inline const TRef<Rendering::COrthographicCamera> &GetCamera() const { return m_orthographic_camera; }

        void Update(float delta);
    };

}