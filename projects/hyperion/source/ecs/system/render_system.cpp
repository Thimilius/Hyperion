//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/system/render_system.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/core/app/display.hpp"
#include "hyperion/ecs/component/components.hpp"
#include "hyperion/ecs/world/world.hpp"
#include "hyperion/rendering/render_engine.hpp"
#include "hyperion/rendering/view/render_view.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    void CameraSystem::Run(World *world) {
        RenderView *render_view = RenderEngine::GetMainRenderView();

        auto view = world->GetView<TransformComponent, CameraComponent>();
        for (EntityId entity : view) {
            TransformComponent *transform = world->GetComponent<TransformComponent>(entity);
            CameraComponent *camera = world->GetComponent<CameraComponent>(entity);

            CameraProjectionMode projection_mode = camera->projection_mode;
            Vector3 position = transform->position;
            Vector3 up = transform->rotation * Vector3::Up();
            Vector3 forward = transform->rotation * Vector3::Forward();
            float32 fov = camera->fov;
            float32 orthographic_size = camera->orthographic_size;
            float32 near_plane = camera->near_plane;
            float32 far_plane = camera->far_plane;
            uint32 width = Display::GetWidth();
            uint32 height = Display::GetHeight();
            float32 aspect_ratio = static_cast<float32>(width) / static_cast<float32>(height);

            Matrix4x4 view_matrix = Matrix4x4::LookAt(position, position + forward, up);
            Matrix4x4 projection_matrix;
            switch (projection_mode) {
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

            RenderViewCameraData &view_camera_data = render_view->AddRenderCamera();
            view_camera_data.projection_mode = projection_mode;
            view_camera_data.position = transform->position;
            view_camera_data.forward = forward;
            view_camera_data.up = up;
            view_camera_data.fov = fov;
            view_camera_data.orthographic_size = orthographic_size;
            view_camera_data.near_plane = near_plane;
            view_camera_data.far_plane = far_plane;
            view_camera_data.view_matrix = view_matrix;
            view_camera_data.inverse_view_matrix = view_matrix.Inverted();
            view_camera_data.projection_matrix = projection_matrix;
            view_camera_data.inverse_projection_matrix = projection_matrix.Inverted();
            view_camera_data.view_projection_matrix = view_projection_matrix;
            view_camera_data.inverse_view_projection_matrix = view_projection_matrix.Inverted();
        }
    }

}