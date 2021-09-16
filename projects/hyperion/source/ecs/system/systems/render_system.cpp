//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/system/systems/render_system.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/core/app/display.hpp"
#include "hyperion/ecs/component/components/render_components.hpp"
#include "hyperion/ecs/component/components/transform_components.hpp"
#include "hyperion/ecs/utilities/camera_utilities.hpp"
#include "hyperion/ecs/utilities/transform_utilities.hpp"
#include "hyperion/ecs/world/world.hpp"
#include "hyperion/rendering/render_engine.hpp"
#include "hyperion/rendering/frame/render_frame.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    void CameraSystem::Run(World *world) {
        RenderFrame *render_frame = RenderEngine::GetMainRenderFrame();

        auto view = world->GetView<DerivedTransformComponent, CameraComponent>();
        for (EntityId entity : view) {
            DerivedTransformComponent *derived_transform = world->GetComponent<DerivedTransformComponent>(entity);
            CameraComponent *camera = world->GetComponent<CameraComponent>(entity);

            CameraUtilities::RecalculateMatricies(camera, derived_transform);

            uint32 display_width = Display::GetWidth();
            uint32 display_height = Display::GetHeight();
            CameraViewportClipping &viewport_clipping = camera->viewport_clipping;
            CameraViewport viewport;
            viewport.x = static_cast<uint32>(Math::Clamp01(viewport_clipping.x) * display_width);
            viewport.y = static_cast<uint32>(Math::Clamp01(viewport_clipping.y) * display_height);
            viewport.width = static_cast<uint32>(Math::Clamp01(viewport_clipping.width) * display_width);
            viewport.height = static_cast<uint32>(Math::Clamp01(viewport_clipping.height) * display_height);

            RenderFrameCamera &render_frame_camera = render_frame->AddCamera();
            render_frame_camera.projection_mode = camera->projection_mode;
            render_frame_camera.clear_mode = camera->clear_mode;
            render_frame_camera.background_color = camera->background_color;
            render_frame_camera.visibility_mask = camera->visibility_mask;
            render_frame_camera.position = derived_transform->position;
            render_frame_camera.forward = TransformUtilities::GetForward(derived_transform);
            render_frame_camera.up = TransformUtilities::GetUp(derived_transform);
            render_frame_camera.fov = camera->fov;
            render_frame_camera.orthographic_size = camera->orthographic_size;
            render_frame_camera.near_plane = camera->near_plane;
            render_frame_camera.far_plane = camera->far_plane;
            render_frame_camera.view_matrix = camera->view_matrix;
            render_frame_camera.inverse_view_matrix = camera->view_matrix.Inverted();
            render_frame_camera.projection_matrix = camera->projection_matrix;
            render_frame_camera.inverse_projection_matrix = camera->projection_matrix.Inverted();
            render_frame_camera.view_projection_matrix = camera->view_projection_matrix;
            render_frame_camera.inverse_view_projection_matrix = camera->view_projection_matrix.Inverted();
            render_frame_camera.viewport = { 0, 0, display_width, display_height };
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

            RenderFrameMeshObject &render_frame_mesh_object = render_frame->AddMeshObject();
            render_frame_mesh_object.local_to_world = local_to_world->local_to_world;
            render_frame_mesh_object.mesh = render_mesh->mesh;
            render_frame_mesh_object.sub_mesh_index = render_mesh->sub_mesh_index;
            render_frame_mesh_object.material = render_mesh->material;
            render_frame_mesh_object.layer_mask = render_mesh->layer_mask;
        }
    }

}