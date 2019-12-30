#include "hyppch.hpp"

#include "hyperion/rendering/camera.hpp"

#include "hyperion/core/app/application.hpp"

namespace Hyperion::Rendering {

    Ray Camera::ScreenPointToRay(Vec2 screen_point) {
        Ref<Window> window = Application::GetInstance()->GetWindow();
        f32 window_width = (f32)window->GetWidth();
        f32 window_height = (f32)window->GetHeight();

        Vec3 position;
        Vec3 direction;
        switch (m_mode) {
            case CameraMode::Perspective: {
                f32 x = (2.0f * screen_point.x) / window_width - 1;
                f32 y = (2.0f * screen_point.y) / window_height - 1;
                Vec2 ndc = Vec2(x, -y);
                Vec4 clip = Vec4(ndc.x, ndc.y, -1.0f, 1.0f);
                Vec4 view = m_inverse_projection_matrix * clip;
                view.z = -1.0f;
                view.w = 0.0f;
                Vec4 world = m_inverse_view_matrix * view;

                position = m_position;
                direction = Vec3(world.x, world.y, world.z);
                break;
            }
            case CameraMode::Orthographic: {
                // TODO: Implement correct orthographic ray projection!
                //position = (screen_point.x / window_width) * (right_bottom - left_bottom) + (screen_point.y / window_height) * (left_top - left_bottom);
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        return Ray(position, direction);
    }

    void Camera::RecalculateMatricies() {
        Ref<Window> window = Application::GetInstance()->GetWindow();
        f32 aspect_ratio = (f32)window->GetWidth() / (f32)window->GetHeight();

        m_view_matrix = Mat4::LookAt(m_position, m_position + m_forward, m_up);
        switch (m_mode) {
            case CameraMode::Perspective: {
                m_projection_matrix = Mat4::Perspective(m_fov, aspect_ratio, m_near_plane, m_far_plane);
                break;
            }
            case CameraMode::Orthographic: {
                m_projection_matrix = Mat4::Orthographic(-m_size * aspect_ratio, m_size * aspect_ratio, -m_size, m_size, m_near_plane, m_far_plane);
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
        m_view_projection_matrix = m_projection_matrix * m_view_matrix;

        m_inverse_view_matrix = m_view_matrix.Inverted();
        m_inverse_projection_matrix = m_projection_matrix.Inverted();
    }

    Ref<Camera> Camera::Create() {
        return Ref<Camera>(new Camera());
    }

}