//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/component/components/utilities/camera_utilities.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/display.hpp"
#include "hyperion/core/math/vector4.hpp"
#include "hyperion/ecs/component/components/utilities/transform_utilities.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

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
            case CameraProjectionMode::Perspective: {
                float32 ndc_x = (2.0f * screen_point.x) / display_width - 1.0f;
                float32 ndc_y = (2.0f * screen_point.y) / display_height - 1.0f;
                Vector4 clip = Vector4(ndc_x, ndc_y, -1.0f, 1.0f);
                Vector4 view = camera->inverse_projection_matrix * clip;
                view.z = -1.0f;
                view.w = 0.0f;
                Vector4 world = camera->inverse_view_matrix * view;

                return Ray(derived_transform->position, Vector3(world.x, world.y, world.z));
            }
            case CameraProjectionMode::Orthographic: {
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
        CameraViewport viewport = CalculateViewportFromClipping(camera->viewport_clipping);
        float32 aspect_ratio = static_cast<float32>(viewport.width) / static_cast<float32>(viewport.height);

        Matrix4x4 view_matrix = Matrix4x4::LookAt(position, position + forward, up);
        Matrix4x4 projection_matrix;
        switch (camera->projection_mode) {
            case CameraProjectionMode::Perspective: {
                projection_matrix = Matrix4x4::Perspective(fov, aspect_ratio, near_plane, far_plane);
                break;
            }
            case CameraProjectionMode::Orthographic: {
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

    //--------------------------------------------------------------
    CameraViewport CameraUtilities::CalculateViewportFromClipping(CameraViewportClipping viewport_clipping) {
        uint32 display_width = Display::GetWidth();
        uint32 display_height = Display::GetHeight();

        CameraViewport viewport;
        viewport.x = static_cast<uint32>(Math::Clamp01(viewport_clipping.x) * display_width);
        viewport.y = static_cast<uint32>(Math::Clamp01(viewport_clipping.y) * display_height);
        viewport.width = static_cast<uint32>(Math::Clamp01(viewport_clipping.width) * display_width);
        viewport.height = static_cast<uint32>(Math::Clamp01(viewport_clipping.height) * display_height);
        return viewport;
    }
    
    //--------------------------------------------------------------
    Array<Plane> CameraUtilities::ExtractFrustumPlanes(Matrix4x4 view_projection) {
        float32 left_plane[4];
        for (int32 i = 3; i >= 0; i--) {
            left_plane[i] = view_projection.columns[i].w + view_projection.columns[i].x;
        }
        float32 right_plane[4];
        for (int32 i = 3; i >= 0; i--) {
            right_plane[i] = view_projection.columns[i].w - view_projection.columns[i].x;
        }
        float32 bottom_plane[4];
        for (int32 i = 3; i >= 0; i--) {
            bottom_plane[i] = view_projection.columns[i].w + view_projection.columns[i].y;
        }
        float32 top_plane[4];
        for (int32 i = 3; i >= 0; i--) {
            top_plane[i] = view_projection.columns[i].w - view_projection.columns[i].y;
        }
        float32 near_plane[4];
        for (int32 i = 3; i >= 0; i--) {
            near_plane[i] = view_projection.columns[i].w + view_projection.columns[i].z;
        }
        float32 far_plane[4];
        for (int32 i = 3; i >= 0; i--) {
            far_plane[i] = view_projection.columns[i].w - view_projection.columns[i].z;
        }

        // We do not use the constructor of plane because it would normalize our normal and we explicitly want to do that ourselves.
        Array<Plane> frustum_planes;
        frustum_planes.Resize(6);
        frustum_planes[0].normal = Vector3(left_plane[0], left_plane[1], left_plane[2]);
        frustum_planes[0].distance = left_plane[3];
        frustum_planes[1].normal = Vector3(right_plane[0], right_plane[1], right_plane[2]);
        frustum_planes[1].distance = right_plane[3];
        frustum_planes[2].normal = Vector3(top_plane[0], top_plane[1], top_plane[2]);
        frustum_planes[2].distance = top_plane[3];
        frustum_planes[3].normal = Vector3(bottom_plane[0], bottom_plane[1], bottom_plane[2]);
        frustum_planes[3].distance = bottom_plane[3];
        frustum_planes[4].normal = Vector3(near_plane[0], near_plane[1], near_plane[2]);
        frustum_planes[4].distance = near_plane[3];
        frustum_planes[5].normal = Vector3(far_plane[0], far_plane[1], far_plane[2]);
        frustum_planes[5].distance = far_plane[3];

        for (int32 i = 0; i < 6; i++) {
            Plane &plane = frustum_planes[i];

            float32 magnitude = plane.normal.Magnitude();
            plane.normal.x /= magnitude;
            plane.normal.y /= magnitude;
            plane.normal.z /= magnitude;
            plane.distance /= magnitude;
        }

        return frustum_planes;
    }

    //--------------------------------------------------------------
    bool CameraUtilities::IsInsideFrustum(const Array<Plane> &frustum_planes, BoundingBox bounds) {
        HYP_ASSERT(frustum_planes.GetLength() == 6);

        Vector3 furthest_away_from_axis;

        for (size_t i = 0; i < 6; i++) {
            Plane plane = frustum_planes[i];

            if (plane.normal.x < 0.0f) {
                furthest_away_from_axis.x = bounds.min.x;
            } else {
                furthest_away_from_axis.x = bounds.max.x;
            }
            if (plane.normal.y < 0.0f) {
                furthest_away_from_axis.y = bounds.min.y;
            } else {
                furthest_away_from_axis.y = bounds.max.y;
            }
            if (plane.normal.z < 0.0f) {
                furthest_away_from_axis.z = bounds.min.z;
            } else {
                furthest_away_from_axis.z = bounds.max.z;
            }

            if (plane.normal.Dot(furthest_away_from_axis) + plane.distance < 0.0f) {
                return false;
            }
        }

        return true;
    }

}