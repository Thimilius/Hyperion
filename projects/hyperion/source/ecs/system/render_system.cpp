//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/system/render_system.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/core/app/display.hpp"
#include "hyperion/ecs/component/components.hpp"
#include "hyperion/ecs/world/world.hpp"
#include "hyperion/rendering/render_engine.hpp"
#include "hyperion/rendering/frame/render_frame.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    void CameraSystem::Run(World *world) {
        RenderFrame *render_frame = RenderEngine::GetMainRenderFrame();

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
            uint32 display_width = Display::GetWidth();
            uint32 display_height = Display::GetHeight();
            float32 aspect_ratio = static_cast<float32>(display_width) / static_cast<float32>(display_height);

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

            CameraViewportClipping &viewport_clipping = camera->viewport_clipping;
            CameraViewport viewport;
            viewport.x = static_cast<uint32>(Math::Clamp01(viewport_clipping.x) * display_width);
            viewport.y = static_cast<uint32>(Math::Clamp01(viewport_clipping.y) * display_height);
            viewport.width = static_cast<uint32>(Math::Clamp01(viewport_clipping.width) * display_width);
            viewport.height = static_cast<uint32>(Math::Clamp01(viewport_clipping.height) * display_height);

            RenderFrameCameraData &render_frame_camera_data = render_frame->AddFrameCamera();
            render_frame_camera_data.projection_mode = projection_mode;
            render_frame_camera_data.clear_mode = camera->clear_mode;
            render_frame_camera_data.background_color = camera->background_color;
            render_frame_camera_data.position = transform->position;
            render_frame_camera_data.forward = forward;
            render_frame_camera_data.up = up;
            render_frame_camera_data.fov = fov;
            render_frame_camera_data.orthographic_size = orthographic_size;
            render_frame_camera_data.near_plane = near_plane;
            render_frame_camera_data.far_plane = far_plane;
            render_frame_camera_data.view_matrix = view_matrix;
            render_frame_camera_data.inverse_view_matrix = view_matrix.Inverted();
            render_frame_camera_data.projection_matrix = projection_matrix;
            render_frame_camera_data.inverse_projection_matrix = projection_matrix.Inverted();
            render_frame_camera_data.view_projection_matrix = view_projection_matrix;
            render_frame_camera_data.inverse_view_projection_matrix = view_projection_matrix.Inverted();
            render_frame_camera_data.viewport = { 0, 0, display_width, display_height };
        }
    }

    //--------------------------------------------------------------
    void SpriteSystem::Run(World *world) {
        RenderFrame *render_frame = RenderEngine::GetMainRenderFrame();

        auto view = world->GetView<LocalToWorldComponent, SpriteComponent>();
        for (EntityId entity : view) {
            LocalToWorldComponent *local_to_world = world->GetComponent<LocalToWorldComponent>(entity);
            SpriteComponent *sprite = world->GetComponent<SpriteComponent>(entity);
        }
    }

    //--------------------------------------------------------------
    void RenderMeshSystem::Run(World *world) {
        RenderFrame *render_frame = RenderEngine::GetMainRenderFrame();
        auto view = world->GetView<LocalToWorldComponent, RenderMeshComponent>();
        for (EntityId entity : view) {
            LocalToWorldComponent *local_to_world = world->GetComponent<LocalToWorldComponent>(entity);
            RenderMeshComponent *render_mesh = world->GetComponent<RenderMeshComponent>(entity);

            RenderFrameObjectData &render_frame_object_data = render_frame->AddFrameObject();
            render_frame_object_data.local_to_world = local_to_world->local_to_world;
            render_frame_object_data.mesh = render_mesh->mesh;
        }
    }

    //--------------------------------------------------------------
    void RenderAssetUnloadSystem::Run(World *world) {
        RenderFrame *render_frame = RenderEngine::GetMainRenderFrame();

        for (Asset *asset : AssetManager::s_assets_to_unload) {
            render_frame->AddFrameAssetToUnload(asset->GetInfo().id);
        }
    }

}