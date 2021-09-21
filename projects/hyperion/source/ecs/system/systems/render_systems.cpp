//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/system/systems/render_systems.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/ecs/component/components/render_components.hpp"
#include "hyperion/ecs/component/components/transform_components.hpp"
#include "hyperion/ecs/component/components/utilities/camera_utilities.hpp"
#include "hyperion/ecs/component/components/utilities/transform_utilities.hpp"
#include "hyperion/ecs/world/world.hpp"
#include "hyperion/render/render_engine.hpp"
#include "hyperion/render/frame/render_frame.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

    //--------------------------------------------------------------
    void CameraSystem::Run(World *world) {
        HYP_PROFILE_SCOPE("CameraSystem.Run");

        RenderFrameContext &render_frame_context = RenderEngine::GetMainRenderFrame()->GetContext();

        auto view = world->GetView<DerivedTransformComponent, CameraComponent>();
        for (EntityId entity : view) {
            DerivedTransformComponent *derived_transform = world->GetComponent<DerivedTransformComponent>(entity);
            CameraComponent *camera = world->GetComponent<CameraComponent>(entity);

            CameraUtilities::RecalculateMatricies(camera, derived_transform);

            RenderFrameContextCamera &render_frame_context_camera = render_frame_context.AddCamera();
            render_frame_context_camera.projection_mode = camera->projection_mode;
            render_frame_context_camera.clear_mode = camera->clear_mode;
            render_frame_context_camera.background_color = camera->background_color;
            render_frame_context_camera.visibility_mask = camera->visibility_mask;
            render_frame_context_camera.position = derived_transform->position;
            render_frame_context_camera.forward = TransformUtilities::GetForward(derived_transform);
            render_frame_context_camera.up = TransformUtilities::GetUp(derived_transform);
            render_frame_context_camera.fov = camera->fov;
            render_frame_context_camera.orthographic_size = camera->orthographic_size;
            render_frame_context_camera.near_plane = camera->near_plane;
            render_frame_context_camera.far_plane = camera->far_plane;
            render_frame_context_camera.view_matrix = camera->view_matrix;
            render_frame_context_camera.inverse_view_matrix = camera->view_matrix.Inverted();
            render_frame_context_camera.projection_matrix = camera->projection_matrix;
            render_frame_context_camera.inverse_projection_matrix = camera->projection_matrix.Inverted();
            render_frame_context_camera.view_projection_matrix = camera->view_projection_matrix;
            render_frame_context_camera.inverse_view_projection_matrix = camera->view_projection_matrix.Inverted();
            render_frame_context_camera.viewport = CameraUtilities::CalculateViewportFromClipping(camera->viewport_clipping);
        }
    }

    //--------------------------------------------------------------
    void SpriteSystem::Run(World *world) {
        HYP_PROFILE_SCOPE("SpriteSystem.Run");

        RenderFrameContext &render_frame_context = RenderEngine::GetMainRenderFrame()->GetContext();

        auto view = world->GetView<LocalToWorldComponent, SpriteComponent>();
        for (EntityId entity : view) {
            LocalToWorldComponent *local_to_world = world->GetComponent<LocalToWorldComponent>(entity);
            SpriteComponent *sprite = world->GetComponent<SpriteComponent>(entity);
            RenderFrameContextObjectSprite &render_frame_context_sprite_object = render_frame_context.AddSpriteObject();
            render_frame_context_sprite_object.local_to_world = local_to_world->local_to_world;
            render_frame_context_sprite_object.color = sprite->color;
        }
    }

    //--------------------------------------------------------------
    void RenderMeshSystem::Run(World *world) {
        HYP_PROFILE_SCOPE("RenderMeshSystem.Run");

        RenderFrameContext &render_frame_context = RenderEngine::GetMainRenderFrame()->GetContext();

        auto view = world->GetView<LocalToWorldComponent, RenderMeshComponent>();
        for (EntityId entity : view) {
            LocalToWorldComponent *local_to_world = world->GetComponent<LocalToWorldComponent>(entity);
            RenderMeshComponent *render_mesh = world->GetComponent<RenderMeshComponent>(entity);

            RenderFrameContextObjectMesh &render_frame_context_mesh_object = render_frame_context.AddMeshObject();
            render_frame_context_mesh_object.local_to_world = local_to_world->local_to_world;
            render_frame_context_mesh_object.mesh = render_mesh->mesh;
            render_frame_context_mesh_object.sub_mesh_index = render_mesh->sub_mesh_index;
            render_frame_context_mesh_object.material = render_mesh->material;
            render_frame_context_mesh_object.layer_mask = render_mesh->layer_mask;
        }
    }

}