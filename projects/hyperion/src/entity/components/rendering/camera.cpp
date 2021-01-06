#include "hyppch.hpp"

#include "hyperion/entity/components/rendering/camera.hpp"

#include "hyperion/entity/components/transform.hpp"
#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/world.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion {

    // NOTE: We treat window cordinates with (0, 0) at bottom left.

    Rendering::CameraData Camera::GetCameraData() const {
        Transform *transform = GetTransform();
        m_data.position = transform->GetPosition();
        m_data.forward = transform->GetForward();
        m_data.up = transform->GetUp();
        return m_data;
    }

    Vec3 Camera::ScreenToWorldPoint(Vec3 screen_point) const {
        f32 display_width = static_cast<f32>(Display::GetWidth());
        f32 display_height = static_cast<f32>(Display::GetHeight());

        f32 x = screen_point.x / display_width * 2.0f - 1.0f;
        f32 y = screen_point.y / display_width * 2.0f - 1.0f;
        Vec4 world = Vec4(x, y, screen_point.z * 2.0f - 1.0f, 1.0f);
        world = m_data.inverse_view_projection_matrix * world;
        world = world * (1.0f / world.w);

        return world;
    }

    Vec2 Camera::WorldToScreenPoint(Vec3 world_point) const {
        f32 display_width = static_cast<f32>(Display::GetWidth());
        f32 display_height = static_cast<f32>(Display::GetHeight());

        Vec4 ndc = m_data.view_projection_matrix * Vec4(world_point, 1);
        ndc = ndc * (1.0f / ndc.w);
        f32 x = Math::Round(((ndc.x + 1.0f) / 2.0f) * display_width);
        f32 y = Math::Round(((ndc.y + 1.0f) / 2.0f) * display_height);

        return Vec2(x, y);
    }

    Ray Camera::ScreenPointToRay(Vec2 screen_point) const {
        f32 display_width = static_cast<f32>(Display::GetWidth());
        f32 display_height = static_cast<f32>(Display::GetHeight());

        switch (m_data.projection_mode) {
            case CameraProjectionMode::Perspective: {
                f32 ndc_x = (2.0f * screen_point.x) / display_width - 1.0f;
                f32 ndc_y = (2.0f * screen_point.y) / display_height - 1.0f;
                Vec4 clip = Vec4(ndc_x, ndc_y, -1.0f, 1.0f);
                Vec4 view = m_data.inverse_projection_matrix * clip;
                view.z = -1.0f;
                view.w = 0.0f;
                Vec4 world = m_data.inverse_view_matrix * view;

                return Ray(GetTransform()->GetPosition(), Vec3(world.x, world.y, world.z));
            }
            case CameraProjectionMode::Orthographic: {
                f32 aspect_ratio = display_width / display_height;

                f32 l = -m_data.orthographic_size * aspect_ratio;
                f32 r = m_data.orthographic_size * aspect_ratio;
                f32 t = m_data.orthographic_size;
                f32 b = -m_data.orthographic_size;

                f32 ray_x = (screen_point.x - (display_width / 2.0f)) / display_width * (r - l);
                f32 ray_y = (screen_point.y - (display_height / 2.0f)) / display_height * (t - b);

                Transform *transform = GetTransform();
                Vec3 forward = transform->GetForward();

                Vec3 position = transform->GetPosition() + (transform->GetRight() * ray_x) + (transform->GetUp() * ray_y) + (forward * m_data.near_plane);
                return Ray(position, forward);
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return Ray(Vec3(), Vec3());
        }
    }

    void Camera::OnMessage(EntityMessage message) {
        if (message.type == EntityMessageType::TransformChanged) {
            RecalculateMatricies();
        }
    }

    void Camera::OnCreate() {
        Component::OnCreate();

        RecalculateMatricies();

        GetWorld()->AddCamera(this);
        GetEntity()->RegisterMessageListener(this);
    }

    void Camera::OnDestroy() {
        GetWorld()->RemoveCamera(this);
        GetEntity()->UnregisterMessageListener(this);

        Component::OnDestroy();
    }

    void Camera::RecalculateMatricies() {
        f32 aspect_ratio = static_cast<f32>(Display::GetWidth() / static_cast<f32>(Display::GetHeight()));
        Vec3 position = GetTransform()->GetPosition();

        m_data.view_matrix = Mat4::LookAt(position, position + GetTransform()->GetForward(), GetTransform()->GetUp());
        switch (m_data.projection_mode) {
            case CameraProjectionMode::Perspective: {
                m_data.projection_matrix = Mat4::Perspective(m_data.fov, aspect_ratio, m_data.near_plane, m_data.far_plane);
                break;
            }
            case CameraProjectionMode::Orthographic: {
                f32 size = m_data.orthographic_size;
                m_data.projection_matrix = Mat4::Orthographic(-size * aspect_ratio, size * aspect_ratio, -size, size, m_data.near_plane, m_data.far_plane);
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }
        m_data.view_projection_matrix = m_data.projection_matrix * m_data.view_matrix;

        m_data.inverse_view_matrix = m_data.view_matrix.Inverted();
        m_data.inverse_projection_matrix = m_data.projection_matrix.Inverted();
        m_data.inverse_view_projection_matrix = m_data.view_projection_matrix.Inverted();
    }

}

HYP_REFLECT_REGISTER_BEGIN
{
    registration::class_<Camera>("Camera");
}
HYP_REFLECT_REGISTER_END
