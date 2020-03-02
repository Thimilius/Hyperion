#include "hyppch.hpp"

#include "hyperion/entity/components/rendering/camera.hpp"

#include "hyperion/entity/components/transform.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion {

    Rendering::CameraData Camera::GetData() const {
        Transform *transform = GetTransform();
        m_data.position = transform->GetPosition();
        m_data.forward = transform->GetForward();
        m_data.up = transform->GetUp();
        return m_data;
    }

    Ray Camera::ScreenPointToRay(Vec2 screen_point) const {
        Window *window = Application::GetInstance()->GetWindow();
        f32 window_width = (f32)window->GetWidth();
        f32 window_height = (f32)window->GetHeight();

        Vec3 position;
        Vec3 direction;
        switch (m_data.mode) {
            case CameraMode::Perspective: {
                f32 x = (2.0f * screen_point.x) / window_width - 1;
                f32 y = (2.0f * screen_point.y) / window_height - 1;
                Vec2 ndc = Vec2(x, -y);
                Vec4 clip = Vec4(ndc.x, ndc.y, -1.0f, 1.0f);
                Vec4 view = m_data.inverse_projection_matrix * clip;
                view.z = -1.0f;
                view.w = 0.0f;
                Vec4 world = m_data.inverse_view_matrix * view;

                position = GetTransform()->GetPosition();
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
        Window *window = Application::GetInstance()->GetWindow();
        f32 aspect_ratio = (f32)window->GetWidth() / (f32)window->GetHeight();
        Vec3 position = GetTransform()->GetPosition();
        f32 size = m_data.size;

        m_data.view_matrix = Mat4::LookAt(position, position + GetTransform()->GetForward(), GetTransform()->GetUp());
        switch (m_data.mode) {
            case CameraMode::Perspective: {
                m_data.projection_matrix = Mat4::Perspective(m_data.fov, aspect_ratio, m_data.near_plane, m_data.far_plane);
                break;
            }
            case CameraMode::Orthographic: {
                m_data.projection_matrix = Mat4::Orthographic(-size * aspect_ratio, size * aspect_ratio, -size, size, m_data.near_plane, m_data.far_plane);
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
        m_data.view_projection_matrix = m_data.projection_matrix * m_data.view_matrix;

        m_data.inverse_view_matrix = m_data.view_matrix.Inverted();
        m_data.inverse_projection_matrix = m_data.projection_matrix.Inverted();
    }

}