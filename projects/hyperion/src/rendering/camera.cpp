#include "hyppch.hpp"

#include "hyperion/rendering/camera.hpp"

#include "hyperion/app/application.hpp"

namespace Hyperion::Rendering {

    using namespace Math;

    void CPerspectiveCamera::RecalculateMatricies() {
        CWindow *window = CApplication::GetInstance()->GetWindow();
        float aspect_ratio = (float)window->GetWidth() / (float)window->GetHeight();
        m_view_matrix = SMat4::LookAt(m_position, m_position + m_forward, m_up);
        m_projection_matrix = SMat4::Perspective(m_fov, aspect_ratio, m_near_plane, m_far_plane);
        m_view_projection_matrix = m_projection_matrix * m_view_matrix;
    }

    void COrthographicCamera::RecalculateMatricies() {
        CWindow *window = CApplication::GetInstance()->GetWindow();
        float aspect_ratio = (float)window->GetWidth() / (float)window->GetHeight();
        m_view_matrix = SMat4::LookAt(m_position, m_position + m_forward, m_up);
        m_projection_matrix = SMat4::Orthographic(-m_size * aspect_ratio, m_size * aspect_ratio, -m_size, m_size, m_near_plane, m_far_plane);
        m_view_projection_matrix = m_projection_matrix * m_view_matrix;
    }

}