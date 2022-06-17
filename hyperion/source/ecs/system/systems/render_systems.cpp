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
#include "hyperion/render/frame/render_frame.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::UI;

//-------------------- Definition Namespace --------------------
namespace Hyperion::Rendering {

  //--------------------------------------------------------------
  void MeshBoundsSystem::Run(EntityManager *manager) {
    HYP_PROFILE_SCOPE("RenderBoundsSystem.Run");

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
    HYP_PROFILE_SCOPE("EnvironmentSystem.Run");

    RenderFrameContext &render_frame_context = RenderEngine::GetMainRenderFrame()->GetContext();

    const WorldEnvironment &world_environment = manager->GetWorld()->GetEnvironment();
    RenderFrameContextEnvironment &render_frame_context_environment = render_frame_context.GetEnvironment();
    render_frame_context_environment.ambient_light.intensity = world_environment.ambient_light.intensity;
    render_frame_context_environment.ambient_light.color = world_environment.ambient_light.color;
  }

  //--------------------------------------------------------------
  void CameraSystem::Run(EntityManager *manager) {
    HYP_PROFILE_SCOPE("CameraSystem.Run");

    RenderFrameContext &render_frame_context = RenderEngine::GetMainRenderFrame()->GetContext();

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
      render_frame_context_camera.viewport = camera_viewport;
    }
  }

  //--------------------------------------------------------------
  void LightSystem::Run(EntityManager *manager) {
    HYP_PROFILE_SCOPE("LightSystem.Run");

    RenderFrameContext &render_frame_context = RenderEngine::GetMainRenderFrame()->GetContext();

    auto directional_view = manager->GetView<DerivedTransformComponent, DirectionalLightComponent>();
    for (EntityId entity : directional_view) {
      DerivedTransformComponent *derived_transform = manager->GetComponent<DerivedTransformComponent>(entity);
      DirectionalLightComponent *directional_light = manager->GetComponent<DirectionalLightComponent>(entity);

      RenderFrameContextLight &render_frame_context_light = render_frame_context.AddLight();
      render_frame_context_light.type = LightType::Directional;
      render_frame_context_light.intensity = directional_light->intensity;
      render_frame_context_light.color = directional_light->color;
      render_frame_context_light.direction = TransformUtilities::GetForward(derived_transform);
    }

    auto point_view = manager->GetView<DerivedTransformComponent, PointLightComponent>();
    for (EntityId entity : point_view) {
      DerivedTransformComponent *derived_transform = manager->GetComponent<DerivedTransformComponent>(entity);
      PointLightComponent *point_light = manager->GetComponent<PointLightComponent>(entity);

      RenderFrameContextLight &render_frame_context_light = render_frame_context.AddLight();
      render_frame_context_light.type = LightType::Point;
      render_frame_context_light.intensity = point_light->intensity;
      render_frame_context_light.color = point_light->color;
      render_frame_context_light.position = derived_transform->position;
      render_frame_context_light.range = point_light->range;
    }

    auto spot_view = manager->GetView<DerivedTransformComponent, SpotLightComponent>();
    for (EntityId entity : spot_view) {
      DerivedTransformComponent *derived_transform = manager->GetComponent<DerivedTransformComponent>(entity);
      SpotLightComponent *spot_light = manager->GetComponent<SpotLightComponent>(entity);

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
  void SpriteRenderSystem::Run(EntityManager *manager) {
    HYP_PROFILE_SCOPE("SpriteSystem.Run");

    RenderFrameContext &render_frame_context = RenderEngine::GetMainRenderFrame()->GetContext();

    auto view = manager->GetView<LocalToWorldComponent, SpriteComponent>();
    for (EntityId entity : view) {
      LocalToWorldComponent *local_to_world = manager->GetComponent<LocalToWorldComponent>(entity);
      SpriteComponent *sprite = manager->GetComponent<SpriteComponent>(entity);

      RenderFrameContextObjectSprite &render_frame_context_sprite_object = render_frame_context.AddSpriteObject();
      render_frame_context_sprite_object.id = EntityId::CreateSmall(entity);
      render_frame_context_sprite_object.local_to_world = local_to_world->local_to_world;
      render_frame_context_sprite_object.position = Vector3(local_to_world->local_to_world.columns[3]);
      render_frame_context_sprite_object.color = sprite->color;
    }
  }

  //--------------------------------------------------------------
  void MeshRenderSystem::Run(EntityManager *manager) {
    HYP_PROFILE_SCOPE("RenderMeshSystem.Run");

    RenderFrameContext &render_frame_context = RenderEngine::GetMainRenderFrame()->GetContext();

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
      
      RenderFrameContextObjectMesh &render_frame_context_mesh_object = render_frame_context.AddMeshObject();
      render_frame_context_mesh_object.id = EntityId::CreateSmall(entity);
      render_frame_context_mesh_object.local_to_world = local_to_world->local_to_world;
      render_frame_context_mesh_object.position = Vector3(local_to_world->local_to_world.columns[3]);
      render_frame_context_mesh_object.mesh_handle = mesh_handle;
      render_frame_context_mesh_object.sub_mesh_index = mesh->sub_mesh_index;
      render_frame_context_mesh_object.shader_handle = AssetManager::GetMaterial(material_handle)->GetShader()->GetMetadata().handle; // TODO: Remove this.
      render_frame_context_mesh_object.material_handle = material_handle;
      render_frame_context_mesh_object.layer_mask = mesh->layer_mask;
      render_frame_context_mesh_object.bounds = mesh_bounds->bounds;
    }
  }

  //--------------------------------------------------------------
  void UIRenderSystem::Run(EntityManager *manager) {
    HYP_PROFILE_SCOPE("UIRenderSystem.Run");

    RenderFrameContext &render_frame_context = RenderEngine::GetMainRenderFrame()->GetContext();
    Delegate<RenderFrameContextObjectUI &()> ui_object_adder;
    ui_object_adder.Connect<&RenderFrameContext::AddUIObject>(&render_frame_context);

    auto view = manager->GetView<UIViewComponent>();
    for (EntityId entity : view) {
      UIViewComponent *ui_view = manager->GetComponent<UIViewComponent>(entity);

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
          AssetHandle texture_handle = renderer.texture
           ? renderer.texture->GetMetadata().handle
           : AssetManager::GetTexture2DPrimitive(Texture2DPrimitive::White)->GetMetadata().handle;

          RenderFrameContextObjectUI &render_frame_context_ui_object = ui_object_adder();
          render_frame_context_ui_object.local_to_world = Matrix4x4::Identity();
          render_frame_context_ui_object.mesh_handle = renderer.mesh->GetMetadata().handle;
          render_frame_context_ui_object.shader_handle = material->GetShader()->GetMetadata().handle;
          render_frame_context_ui_object.material_handle = material->GetMetadata().handle;
          render_frame_context_ui_object.color = color;
          render_frame_context_ui_object.texture.handle = texture_handle;
          render_frame_context_ui_object.texture.dimension = renderer.texture ? renderer.texture->GetDimension() : TextureDimension::Texture2D;
          render_frame_context_ui_object.texture.render_texture_attachment_index = renderer.render_texture_attachment_index;
          render_frame_context_ui_object.scissor = { 0, 0, static_cast<int32>(Display::GetWidth()), static_cast<int32>(Display::GetHeight()) };
          render_frame_context_ui_object.enable_blending = renderer.enable_blending;
        }
      }

      for (UIElement *child : element->GetHierarchy().GetChildren()) {
        RenderElement(child, ui_object_adder);
      }
    }
  }

}
