#include "hyppch.hpp"

#include "hyperion/rendering/camera.hpp"

#include "hyperion/core/app/application.hpp"

namespace Hyperion::Rendering {

    Vec2 Camera::ScreenPointToNDC(Vec2 screen_point) {
        Window *window = Application::GetInstance()->GetWindow();
        f32 x = (2.0f * screen_point.x) / window->GetWidth() - 1;
        f32 y = (2.0f * screen_point.y) / window->GetHeight() - 1;
        return Vec2(x, -y);
    }

    Ray Camera::ScreenPointToRay(Vec2 screen_point) {
        Vec2 ndc = ScreenPointToNDC(screen_point);
        Vec4 clip = Vec4(ndc.x, ndc.y, -1.0f, 1.0f);
        Vec4 view = m_inverse_projection_matrix * clip;
        view.z = -1.0f;
        view.w = 0.0f;
        Vec4 world = m_inverse_view_matrix * view;
        Vec3 direction = Vec3(world.x, world.y, world.z);

        return Ray(m_position, direction);
    }

    void PerspectiveCamera::RecalculateMatricies() {
        Window *window = Application::GetInstance()->GetWindow();
        f32 aspect_ratio = (f32)window->GetWidth() / (f32)window->GetHeight();
        m_view_matrix = Mat4::LookAt(m_position, m_position + m_forward, m_up);
        m_projection_matrix = Mat4::Perspective(m_fov, aspect_ratio, m_near_plane, m_far_plane);
        m_view_projection_matrix = m_projection_matrix * m_view_matrix;
        m_inverse_view_matrix = m_view_matrix.Inverted();
        m_inverse_projection_matrix = m_projection_matrix.Inverted();
    }

    void OrthographicCamera::RecalculateMatricies() {
        Window *window = Application::GetInstance()->GetWindow();
        f32 aspect_ratio = (f32)window->GetWidth() / (f32)window->GetHeight();
        m_view_matrix = Mat4::LookAt(m_position, m_position + m_forward, m_up);
        m_projection_matrix = Mat4::Orthographic(-m_size * aspect_ratio, m_size * aspect_ratio, -m_size, m_size, m_near_plane, m_far_plane);
        m_view_projection_matrix = m_projection_matrix * m_view_matrix;
        m_inverse_view_matrix = m_view_matrix.Inverted();
        m_inverse_projection_matrix = m_projection_matrix.Inverted();
    }

}