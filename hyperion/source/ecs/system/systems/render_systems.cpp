//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/system/systems/render_systems.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/core/app/display.hpp"
#include "hyperion/ecs/component/components/core_components.hpp"
#include "hyperion/ecs/component/components/render_components.hpp"
#include "hyperion/ecs/component/components/transform_components.hpp"
#include "hyperion/ecs/component/components/ui_components.hpp"
#include "hyperion/ecs/component/components/utilities/camera_utilities.hpp"
#include "hyperion/ecs/component/components/utilities/transform_utilities.hpp"
#include "hyperion/ecs/world/world.hpp"
#include "hyperion/render/render_engine.hpp"
#include "hyperion/render/render_frame.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::UI;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  void MeshBoundsSystem::Run(EntityManager *manager) {
    HYP_PROFILE_SCOPE("RenderBoundsSystem.Run")

    auto view = manager->GetView<LocalToWorldComponent, MeshComponent, MeshBoundsComponent>(ExcludeComponents<StaticComponent>());
    for (EntityId entity : view) {
      LocalToWorldComponent *local_to_world = manager->GetComponent<LocalToWorldComponent>(entity);
      MeshComponent *mesh = manager->GetComponent<MeshComponent>(entity);
      MeshBoundsComponent *mesh_bounds = manager->GetComponent<MeshBoundsComponent>(entity);

      AssetHandle mesh_handle = mesh->mesh;
      if (mesh_handle.IsValid()) {
        mesh_bounds->bounds = BoundingBox::Transform(local_to_world->local_to_world, AssetManager::GetMesh(mesh_handle)->GetBounds());  
      }
    }
  }

  //--------------------------------------------------------------
  void EnvironmentSystem::Run(EntityManager *manager) {
    HYP_PROFILE_SCOPE("EnvironmentSystem.Run")

    RenderObjectContext &render_object_context = RenderEngine::GetMainRenderFrame()->GetObjectContext();

    const WorldEnvironment &world_environment = manager->GetWorld()->GetEnvironment();
    RenderObjectContextEnvironment &render_object_context_environment = render_object_context.GetEnvironment();
    render_object_context_environment.ambient_light.intensity = world_environment.ambient_light.intensity;
    render_object_context_environment.ambient_light.color = world_environment.ambient_light.color;
  }

  //--------------------------------------------------------------
  void CameraSystem::Run(EntityManager *manager) {
    HYP_PROFILE_SCOPE("CameraSystem.Run")

    RenderObjectContext &render_object_context = RenderEngine::GetMainRenderFrame()->GetObjectContext();

    auto view = manager->GetView<DerivedTransformComponent, CameraComponent>();
    uint32 index = 0;
    for (EntityId entity : view) {
      DerivedTransformComponent *derived_transform = manager->GetComponent<DerivedTransformComponent>(entity);
      CameraComponent *camera = manager->GetComponent<CameraComponent>(entity);

#ifdef HYP_EDITOR
      Vector2 display_size = Vector2(static_cast<float32>(Display::GetPreviewWidth()), static_cast<float32>(Display::GetPreviewHeight()));
#else
      Vector2 display_size = Vector2(static_cast<float32>(Display::GetWidth()), static_cast<float32>(Display::GetHeight()));
#endif
      CameraViewport camera_viewport = CameraUtilities::CalculateViewportFromClipping(camera->viewport_clipping, display_size);
      CameraUtilities::RecalculateMatrices(camera, derived_transform, camera_viewport);

      RenderObjectContextCamera &render_object_context_camera = render_object_context.AddCamera();
      render_object_context_camera.index = index++;
      render_object_context_camera.projection_mode = camera->projection_mode;
      render_object_context_camera.clear_mode = camera->clear_mode;
      render_object_context_camera.background_color = camera->background_color;
      render_object_context_camera.culling_mask = camera->culling_mask;
      render_object_context_camera.position = derived_transform->position;
      render_object_context_camera.forward = TransformUtilities::GetForward(derived_transform);
      render_object_context_camera.up = TransformUtilities::GetUp(derived_transform);
      render_object_context_camera.fov = camera->fov;
      render_object_context_camera.orthographic_size = camera->orthographic_size;
      render_object_context_camera.near_plane = camera->near_plane;
      render_object_context_camera.far_plane = camera->far_plane;
      render_object_context_camera.view_matrix = camera->view_matrix;
      render_object_context_camera.inverse_view_matrix = camera->view_matrix.Inverted();
      render_object_context_camera.projection_matrix = camera->projection_matrix;
      render_object_context_camera.inverse_projection_matrix = camera->projection_matrix.Inverted();
      render_object_context_camera.view_projection_matrix = camera->view_projection_matrix;
      render_object_context_camera.inverse_view_projection_matrix = camera->view_projection_matrix.Inverted();
      render_object_context_camera.viewport = camera_viewport;
    }
  }

  //--------------------------------------------------------------
  void LightSystem::Run(EntityManager *manager) {
    HYP_PROFILE_SCOPE("LightSystem.Run")

    RenderObjectContext &render_object_context = RenderEngine::GetMainRenderFrame()->GetObjectContext();

    auto directional_view = manager->GetView<DerivedTransformComponent, DirectionalLightComponent>();
    for (EntityId entity : directional_view) {
      DerivedTransformComponent *derived_transform = manager->GetComponent<DerivedTransformComponent>(entity);
      DirectionalLightComponent *directional_light = manager->GetComponent<DirectionalLightComponent>(entity);

      RenderObjectContextLight &render_object_context_light = render_object_context.AddLight();
      render_object_context_light.type = LightType::Directional;
      render_object_context_light.intensity = directional_light->intensity;
      render_object_context_light.color = directional_light->color;
      render_object_context_light.shadows = directional_light->shadows;
      render_object_context_light.shadow_intensity = directional_light->shadow_intensity;
      render_object_context_light.shadow_bias = directional_light->shadow_bias;
      render_object_context_light.direction = TransformUtilities::GetForward(derived_transform);
    }

    auto point_view = manager->GetView<DerivedTransformComponent, PointLightComponent>();
    for (EntityId entity : point_view) {
      DerivedTransformComponent *derived_transform = manager->GetComponent<DerivedTransformComponent>(entity);
      PointLightComponent *point_light = manager->GetComponent<PointLightComponent>(entity);

      RenderObjectContextLight &render_object_context_light = render_object_context.AddLight();
      render_object_context_light.type = LightType::Point;
      render_object_context_light.intensity = point_light->intensity;
      render_object_context_light.color = point_light->color;
      render_object_context_light.shadows = point_light->shadows;
      render_object_context_light.shadow_intensity = point_light->shadow_intensity;
      render_object_context_light.shadow_bias = point_light->shadow_bias;
      render_object_context_light.position = derived_transform->position;
      render_object_context_light.range = point_light->range;
    }

    auto spot_view = manager->GetView<DerivedTransformComponent, SpotLightComponent>();
    for (EntityId entity : spot_view) {
      DerivedTransformComponent *derived_transform = manager->GetComponent<DerivedTransformComponent>(entity);
      SpotLightComponent *spot_light = manager->GetComponent<SpotLightComponent>(entity);

      RenderObjectContextLight &render_object_context_light = render_object_context.AddLight();
      render_object_context_light.type = LightType::Spot;
      render_object_context_light.intensity = spot_light->intensity;
      render_object_context_light.color = spot_light->color;
      render_object_context_light.shadows = spot_light->shadows;
      render_object_context_light.shadow_intensity = spot_light->shadow_intensity;
      render_object_context_light.shadow_bias = spot_light->shadow_bias;
      render_object_context_light.position = derived_transform->position;
      render_object_context_light.inner_spot_radius = spot_light->inner_spot_radius;
      render_object_context_light.outer_spot_radius = spot_light->outer_spot_radius;
    }
  }

  //--------------------------------------------------------------
  void SpriteRenderSystem::Run(EntityManager *manager) {
    HYP_PROFILE_SCOPE("SpriteSystem.Run")

    RenderObjectContext &render_object_context = RenderEngine::GetMainRenderFrame()->GetObjectContext();

    auto view = manager->GetView<LocalToWorldComponent, SpriteComponent>();
    for (EntityId entity : view) {
      LocalToWorldComponent *local_to_world = manager->GetComponent<LocalToWorldComponent>(entity);
      SpriteComponent *sprite = manager->GetComponent<SpriteComponent>(entity);

      RenderObjectContextSprite &render_object_context_sprite = render_object_context.AddSprite();
      render_object_context_sprite.id = entity;
      render_object_context_sprite.local_to_world = local_to_world->local_to_world;
      render_object_context_sprite.position = Vector3(local_to_world->local_to_world.columns[3]);
      render_object_context_sprite.color = sprite->color;
    }
  }

  //--------------------------------------------------------------
  void MeshRenderSystem::Run(EntityManager *manager) {
    HYP_PROFILE_SCOPE("RenderMeshSystem.Run")

    RenderObjectContext &render_object_context = RenderEngine::GetMainRenderFrame()->GetObjectContext();

    auto view = manager->GetView<LocalToWorldComponent, MeshBoundsComponent, MeshComponent>();
    for (EntityId entity : view) {
      LocalToWorldComponent *local_to_world = manager->GetComponent<LocalToWorldComponent>(entity);
      MeshBoundsComponent *mesh_bounds = manager->GetComponent<MeshBoundsComponent>(entity);
      MeshComponent *mesh = manager->GetComponent<MeshComponent>(entity);

      AssetHandle mesh_handle = mesh->mesh;
      AssetHandle material_handle = mesh->material;
      if (!mesh_handle.IsValid() || !material_handle.IsValid()) {
        continue;
      }
      
      RenderObjectContextMesh &render_object_context_mesh = render_object_context.AddMesh();
      render_object_context_mesh.id = entity;
      render_object_context_mesh.local_to_world = local_to_world->local_to_world;
      render_object_context_mesh.position = Vector3(local_to_world->local_to_world.columns[3]);
      render_object_context_mesh.mesh_handle = mesh_handle;
      render_object_context_mesh.sub_mesh_index = mesh->sub_mesh_index;
      render_object_context_mesh.material_handle = material_handle;
      render_object_context_mesh.layer_mask = mesh->layer_mask;
      render_object_context_mesh.bounds = mesh_bounds->bounds;
    }
  }

  //--------------------------------------------------------------
  void UIRenderSystem::Run(EntityManager *manager) {
    HYP_PROFILE_SCOPE("UIRenderSystem.Run")

    RenderObjectContext &render_object_context = RenderEngine::GetMainRenderFrame()->GetObjectContext();
    Delegate<RenderObjectContextUIElement &()> ui_object_adder;
    ui_object_adder.Connect<&RenderObjectContext::AddUIElement>(&render_object_context);

    auto view = manager->GetView<UIViewComponent>();
    for (EntityId entity : view) {
      UIViewComponent *ui_view = manager->GetComponent<UIViewComponent>(entity);

      Run(ui_view, ui_object_adder);
    }
  }

  //--------------------------------------------------------------
  void UIRenderSystem::Run(UIViewComponent *ui_view, Delegate<RenderObjectContextUIElement &()> ui_object_adder) {
    RenderElement(ui_view->root_element, ui_object_adder);
  }

  //--------------------------------------------------------------
  void UIRenderSystem::RenderElement(UIElement *element, Delegate<RenderObjectContextUIElement &()> ui_object_adder) {
    HYP_PROFILE_SCOPE("UIRenderSystem.RenderElement")

    if (element && element->GetStyle().GetVisibility() == Visibility::Visible) {
      UIElementRenderer renderer = element->GetRenderer();

      Mesh *mesh = renderer.mesh;
      if (mesh != nullptr) {
        float32 opacity = element->GetStyle().GetOpacity();

        Color color = renderer.color;
        color.a *= opacity;

        if (color.a > 0.0f) {
          Material *material = renderer.material ? renderer.material : AssetManager::GetMaterialPrimitive(MaterialPrimitive::UI);
          AssetHandle texture_handle = renderer.texture
           ? renderer.texture->GetHandle()
           : AssetManager::GetTexture2DPrimitive(Texture2DPrimitive::White)->GetHandle();

          RenderObjectContextUIElement &render_object_context_ui_element = ui_object_adder();
          render_object_context_ui_element.local_to_world = Matrix4x4::Identity();
          render_object_context_ui_element.mesh_handle = renderer.mesh->GetHandle();
          render_object_context_ui_element.shader_handle = material->GetShader()->GetHandle();
          render_object_context_ui_element.material_handle = material->GetHandle();
          render_object_context_ui_element.color = color;
          render_object_context_ui_element.texture.handle = texture_handle;
          render_object_context_ui_element.texture.dimension = renderer.texture ? renderer.texture->GetDimension() : TextureDimension::Texture2D;
          render_object_context_ui_element.texture.render_texture_attachment_index = renderer.render_texture_attachment_index;
          render_object_context_ui_element.scissor = { 0, 0, static_cast<int32>(Display::GetWidth()), static_cast<int32>(Display::GetHeight()) };
          render_object_context_ui_element.enable_blending = renderer.enable_blending;
        }
      }

      for (UIElement *child : element->GetHierarchy().GetChildren()) {
        RenderElement(child, ui_object_adder);
      }
    }
  }

}
