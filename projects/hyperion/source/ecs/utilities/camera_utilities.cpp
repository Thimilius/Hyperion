//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/component/components/utilities/camera_utilities.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/display.hpp"
#include "hyperion/core/math/vector4.hpp"
#include "hyperion/ecs/component/components/utilities/transform_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    Vector3 CameraUtilities::ScreenToWorldPoint(CameraComponent *camera, Vector3 screen_point) {
        float32 display_width = static_cast<float32>(Display::GetWidth());
        float32 display_height = static_cast<float32>(Display::GetHeight());

        float32 x = screen_point.x / display_width * 2.0f - 1.0f;
        float32 y = screen_point.y / display_width * 2.0f - 1.0f;
        Vector4 world = Vector4(x, y, screen_point.z * 2.0f - 1.0f, 1.0f);
        world = camera->inverse_view_projection_matrix * world;
        world = world * (1.0f / world.w);

        return world;
    }

    //--------------------------------------------------------------
    Vector2 CameraUtilities::WorldToScreenPoint(CameraComponent *camera, Vector3 world_point) {
        float32 display_width = static_cast<float32>(Display::GetWidth());
        float32 display_height = static_cast<float32>(Display::GetHeight());

        Vector4 ndc = camera->view_projection_matrix * Vector4(world_point, 1);
        ndc = ndc * (1.0f / ndc.w);
        float32 x = Math::Round(((ndc.x + 1.0f) / 2.0f) * display_width);
        float32 y = Math::Round(((ndc.y + 1.0f) / 2.0f) * display_height);

        return Vector2(x, y);
    }

    //--------------------------------------------------------------
    Ray CameraUtilities::ScreenPointToRay(CameraComponent *camera, DerivedTransformComponent *derived_transform, Vector2 screen_point) {
        float32 display_width = static_cast<float32>(Display::GetWidth());
        float32 display_height = static_cast<float32>(Display::GetHeight());

        switch (camera->projection_mode) {
            case Rendering::CameraProjectionMode::Perspective: {
                float32 ndc_x = (2.0f * screen_point.x) / display_width - 1.0f;
                float32 ndc_y = (2.0f * screen_point.y) / display_height - 1.0f;
                Vector4 clip = Vector4(ndc_x, ndc_y, -1.0f, 1.0f);
                Vector4 view = camera->inverse_projection_matrix * clip;
                view.z = -1.0f;
                view.w = 0.0f;
                Vector4 world = camera->inverse_view_matrix * view;

                return Ray(derived_transform->position, Vector3(world.x, world.y, world.z));
            }
            case Rendering::CameraProjectionMode::Orthographic: {
                float32 aspect_ratio = display_width / display_height;

                float32 l = -camera->orthographic_size * aspect_ratio;
                float32 r = camera->orthographic_size * aspect_ratio;
                float32 t = camera->orthographic_size;
                float32 b = -camera->orthographic_size;

                float32 ray_x = (screen_point.x - (display_width / 2.0f)) / display_width * (r - l);
                float32 ray_y = (screen_point.y - (display_height / 2.0f)) / display_height * (t - b);

                Vector3 right = TransformUtilities::GetRight(derived_transform);
                Vector3 up = TransformUtilities::GetUp(derived_transform);
                Vector3 forward = TransformUtilities::GetForward(derived_transform);

                Vector3 position = derived_transform->position + (right * ray_x) + (up * ray_y) + (forward * camera->near_plane);
                return Ray(position, forward);
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return Ray();
        }
    }

    //--------------------------------------------------------------
    void CameraUtilities::RecalculateMatricies(CameraComponent *camera, DerivedTransformComponent *derived_transform) {
        Vector3 position = derived_transform->position;
        Vector3 up = TransformUtilities::GetUp(derived_transform);
        Vector3 forward = TransformUtilities::GetForward(derived_transform);
        float32 fov = camera->fov;
        float32 orthographic_size = camera->orthographic_size;
        float32 near_plane = camera->near_plane;
        float32 far_plane = camera->far_plane;
        uint32 display_width = Display::GetWidth();
        uint32 display_height = Display::GetHeight();
        float32 aspect_ratio = static_cast<float32>(display_width) / static_cast<float32>(display_height);

        Matrix4x4 view_matrix = Matrix4x4::LookAt(position, position + forward, up);
        Matrix4x4 projection_matrix;
        switch (camera->projection_mode) {
            case Rendering::CameraProjectionMode::Perspective: {
                projection_matrix = Matrix4x4::Perspective(fov, aspect_ratio, near_plane, far_plane);
                break;
            }
            case Rendering::CameraProjectionMode::Orthographic: {
                projection_matrix = Matrix4x4::Orthographic(
                    -orthographic_size * aspect_ratio,
                    orthographic_size * aspect_ratio,
                    -orthographic_size, orthographic_size,
                    near_plane,
                    far_plane);
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
        }
        Matrix4x4 view_projection_matrix = projection_matrix * view_matrix;

        camera->view_matrix = view_matrix;
        camera->inverse_view_matrix = view_matrix.Inverted();
        camera->projection_matrix = projection_matrix;
        camera->inverse_projection_matrix = projection_matrix.Inverted();
        camera->view_projection_matrix = view_projection_matrix;
        camera->inverse_view_projection_matrix = view_projection_matrix.Inverted();
    }

}