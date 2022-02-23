//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/system/systems/render_systems.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/ecs/component/components/core_components.hpp"
#include "hyperion/ecs/component/components/render_components.hpp"
#include "hyperion/ecs/component/components/transform_components.hpp"
#include "hyperion/ecs/component/components/ui_components.hpp"
#include "hyperion/ecs/component/components/utilities/camera_utilities.hpp"
#include "hyperion/ecs/component/components/utilities/transform_utilities.hpp"
#include "hyperion/ecs/world/world.hpp"
#include "hyperion/render/render_engine.hpp"
#include "hyperion/render/frame/render_frame.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::UI;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  void MeshBoundsSystem::Run(World *world) {
    HYP_PROFILE_SCOPE("RenderBoundsSystem.Run");

    auto view = world->GetView<LocalToWorldComponent, LocalMeshBoundsComponent, WorldMeshBoundsComponent>(ExcludeComponents<DisabledComponent, StaticComponent>());
    for (EntityId entity : view) {
      LocalToWorldComponent *local_to_world = world->GetComponent<LocalToWorldComponent>(entity);
      LocalMeshBoundsComponent *local_mesh_bounds = world->GetComponent<LocalMeshBoundsComponent>(entity);
      WorldMeshBoundsComponent *world_mesh_bounds = world->GetComponent<WorldMeshBoundsComponent>(entity);

      world_mesh_bounds->bounds = BoundingBox::Transform(local_to_world->local_to_world, local_mesh_bounds->bounds);
    }
  }

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

    auto view = world->GetView<DerivedTransformComponent, CameraComponent>(ExcludeComponents<DisabledComponent>());
    uint32 index = 0;
    for (EntityId entity : view) {
      DerivedTransformComponent *derived_transform = world->GetComponent<DerivedTransformComponent>(entity);
      CameraComponent *camera = world->GetComponent<CameraComponent>(entity);

      CameraUtilities::RecalculateMatricies(camera, derived_transform);

      RenderFrameContextCamera &render_frame_context_camera = render_frame_context.AddCamera();
      render_frame_context_camera.index = index++;
      render_frame_context_camera.projection_mode = camera->projection_mode;
      render_frame_context_camera.clear_mode = camera->clear_mode;
      render_frame_context_camera.background_color = camera->background_color;
      render_frame_context_camera.culling_mask = camera->culling_mask;
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

    auto directional_view = world->GetView<DerivedTransformComponent, DirectionalLightComponent>(ExcludeComponents<DisabledComponent>());
    for (EntityId entity : directional_view) {
      DerivedTransformComponent *derived_transform = world->GetComponent<DerivedTransformComponent>(entity);
      DirectionalLightComponent *directional_light = world->GetComponent<DirectionalLightComponent>(entity);

      RenderFrameContextLight &render_frame_context_light = render_frame_context.AddLight();
      render_frame_context_light.type = LightType::Directional;
      render_frame_context_light.intensity = directional_light->intensity;
      render_frame_context_light.color = directional_light->color;
      render_frame_context_light.direction = TransformUtilities::GetForward(derived_transform);
    }

    auto point_view = world->GetView<DerivedTransformComponent, PointLightComponent>(ExcludeComponents<DisabledComponent>());
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

    auto spot_view = world->GetView<DerivedTransformComponent, SpotLightComponent>(ExcludeComponents<DisabledComponent>());
    for (EntityId entity : spot_view) {
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
  void SpriteRenderSystem::Run(World *world) {
    HYP_PROFILE_SCOPE("SpriteSystem.Run");

    RenderFrameContext &render_frame_context = RenderEngine::GetMainRenderFrame()->GetContext();

    auto view = world->GetView<LocalToWorldComponent, SpriteComponent>(ExcludeComponents<DisabledComponent>());
    for (EntityId entity : view) {
      LocalToWorldComponent *local_to_world = world->GetComponent<LocalToWorldComponent>(entity);
      SpriteComponent *sprite = world->GetComponent<SpriteComponent>(entity);

      RenderFrameContextObjectSprite &render_frame_context_sprite_object = render_frame_context.AddSpriteObject();
      render_frame_context_sprite_object.id = EntityUtilities::CreateSmallId(entity);
      render_frame_context_sprite_object.local_to_world = local_to_world->local_to_world;
      render_frame_context_sprite_object.position = Vector3(local_to_world->local_to_world.columns[3]);
      render_frame_context_sprite_object.color = sprite->color;
    }
  }

  //--------------------------------------------------------------
  void MeshRenderSystem::Run(World *world) {
    HYP_PROFILE_SCOPE("RenderMeshSystem.Run");

    RenderFrameContext &render_frame_context = RenderEngine::GetMainRenderFrame()->GetContext();

    auto view = world->GetView<LocalToWorldComponent, WorldMeshBoundsComponent, MeshComponent>(ExcludeComponents<DisabledComponent>());
    for (EntityId entity : view) {
      LocalToWorldComponent *local_to_world = world->GetComponent<LocalToWorldComponent>(entity);
      WorldMeshBoundsComponent *world_mesh_bounds = world->GetComponent<WorldMeshBoundsComponent>(entity);
      MeshComponent *mesh = world->GetComponent<MeshComponent>(entity);

      RenderFrameContextObjectMesh &render_frame_context_mesh_object = render_frame_context.AddMeshObject();
      render_frame_context_mesh_object.id = EntityUtilities::GetIndex(entity);
      render_frame_context_mesh_object.local_to_world = local_to_world->local_to_world;
      render_frame_context_mesh_object.position = Vector3(local_to_world->local_to_world.columns[3]);
      render_frame_context_mesh_object.mesh_id = mesh->mesh->GetAssetInfo().id;
      render_frame_context_mesh_object.sub_mesh_index = mesh->sub_mesh_index;
      render_frame_context_mesh_object.shader_id = mesh->material->GetShader()->GetAssetInfo().id;
      render_frame_context_mesh_object.material_id = mesh->material->GetAssetInfo().id;
      render_frame_context_mesh_object.layer_mask = mesh->layer_mask;
      render_frame_context_mesh_object.bounds = world_mesh_bounds->bounds;
    }
  }

  //--------------------------------------------------------------
  void UIRenderSystem::Run(World *world) {
    HYP_PROFILE_SCOPE("UIRenderSystem.Run");

    RenderFrameContext &render_frame_context = RenderEngine::GetMainRenderFrame()->GetContext();
    Delegate<RenderFrameContextObjectUI &()> ui_object_adder;
    ui_object_adder.Connect<&RenderFrameContext::AddUIObject>(&render_frame_context);

    auto view = world->GetView<UIViewComponent>(ExcludeComponents<DisabledComponent>());
    for (EntityId entity : view) {
      UIViewComponent *ui_view = world->GetComponent<UIViewComponent>(entity);

      Run(ui_view, ui_object_adder);
    }
  }

  //--------------------------------------------------------------
  void UIRenderSystem::Run(UIViewComponent *ui_view, Delegate<RenderFrameContextObjectUI &()> ui_object_adder) {
    RenderElement(ui_view->root_element, ui_object_adder);
  }

  //--------------------------------------------------------------
  void UIRenderSystem::RenderElement(UIElement *element, Delegate<RenderFrameContextObjectUI &()> ui_object_adder) {
    HYP_PROFILE_SCOPE("UIRenderSystem.RenderElement");

    if (element && element->GetStyle().GetVisibility() == Visibility::Visible) {
      UIElementRenderer renderer = element->GetRenderer();

      Mesh *mesh = renderer.mesh;
      if (mesh != nullptr) {
        float32 opacity = element->GetStyle().GetOpacity();

        Color color = renderer.color;
        color.a *= opacity;

        if (color.a > 0.0f) {
          Material *material = renderer.material ? renderer.material : AssetManager::GetMaterialPrimitive(MaterialPrimitive::UI);
          AssetId texture_id = renderer.texture ? renderer.texture->GetAssetInfo().id : AssetManager::GetTexture2DPrimitive(Texture2DPrimitive::White)->GetAssetInfo().id;

          RenderFrameContextObjectUI &render_frame_context_ui_object = ui_object_adder();
          render_frame_context_ui_object.local_to_world = Matrix4x4::Identity();
          render_frame_context_ui_object.mesh_id = renderer.mesh->GetAssetInfo().id;
          render_frame_context_ui_object.shader_id = material->GetShader()->GetAssetInfo().id;
          render_frame_context_ui_object.material_id = material->GetAssetInfo().id;
          render_frame_context_ui_object.color = color;
          render_frame_context_ui_object.texture.id = texture_id;
          render_frame_context_ui_object.texture.dimension = renderer.texture ? renderer.texture->GetDimension() : TextureDimension::Texture2D;
          render_frame_context_ui_object.texture.render_texture_attchment_index = renderer.render_texture_attachment_index;
          render_frame_context_ui_object.enable_blending = renderer.enable_blending;
        }
      }

      for (UIElement *child : element->GetHierarchy().GetChildren()) {
        RenderElement(child, ui_object_adder);
      }
    }
  }

}