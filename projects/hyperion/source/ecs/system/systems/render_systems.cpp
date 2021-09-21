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
    void EnvironmentSystem::Run(World *world) {
        HYP_PROFILE_SCOPE("EnvironmentSystem.Run");

        RenderFrameContext &render_frame_context = RenderEngine::GetMainRenderFrame()->GetContext();

        const WorldEnvironment &world_environment = world->GetEnvironment();
        RenderFrameContextEnvironment &render_frame_context_environment = render_frame_context.GetEnvironment();
        render_frame_context_environment.ambient_light.intensity = world_environment.ambient_light.intensity;
        render_frame_context_environment.ambient_light.color = world_environment.ambient_light.color;
    }

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
    void LightSystem::Run(World *world) {
        HYP_PROFILE_SCOPE("LightSystem.Run");

        RenderFrameContext &render_frame_context = RenderEngine::GetMainRenderFrame()->GetContext();

        auto directional_view = world->GetView<DerivedTransformComponent, DirectionalLightComponent>();
        for (EntityId entity : directional_view) {
            DerivedTransformComponent *derived_transform = world->GetComponent<DerivedTransformComponent>(entity);
            DirectionalLightComponent *directional_light = world->GetComponent<DirectionalLightComponent>(entity);

            RenderFrameContextLight &render_frame_context_light = render_frame_context.AddLight();
            render_frame_context_light.type = LightType::Directional;
            render_frame_context_light.intensity = directional_light->intensity;
            render_frame_context_light.color = directional_light->color;
            render_frame_context_light.direction = TransformUtilities::GetForward(derived_transform);
        }

        auto point_view = world->GetView<DerivedTransformComponent, PointLightComponent>();
        for (EntityId entity : point_view) {
            DerivedTransformComponent *derived_transform = world->GetComponent<DerivedTransformComponent>(entity);
            PointLightComponent *point_light = world->GetComponent<PointLightComponent>(entity);

            RenderFrameContextLight &render_frame_context_light = render_frame_context.AddLight();
            render_frame_context_light.type = LightType::Point;
            render_frame_context_light.intensity = point_light->intensity;
            render_frame_context_light.color = point_light->color;
            render_frame_context_light.position = derived_transform->position;
            render_frame_context_light.range = point_light->range;
        }

        auto spot_view = world->GetView<DerivedTransformComponent, SpotLightComponent>();
        for (EntityId entity : point_view) {
            DerivedTransformComponent *derived_transform = world->GetComponent<DerivedTransformComponent>(entity);
            SpotLightComponent *spot_light = world->GetComponent<SpotLightComponent>(entity);

            RenderFrameContextLight &render_frame_context_light = render_frame_context.AddLight();
            render_frame_context_light.type = LightType::Spot;
            render_frame_context_light.intensity = spot_light->intensity;
            render_frame_context_light.color = spot_light->color;
            render_frame_context_light.position = derived_transform->position;
            render_frame_context_light.inner_spot_radius = spot_light->inner_spot_radius;
            render_frame_context_light.outer_spot_radius = spot_light->outer_spot_radius;
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