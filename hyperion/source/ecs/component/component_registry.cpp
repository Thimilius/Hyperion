//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/component/component_registry.hpp"
#include "hyperion/ecs/component/components/components.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  void ComponentRegistry::RegisterTypes() {
    // Basic Types.
    {
      using namespace Rendering;
      using namespace UI;

      Registration::class_<EntityId>(HYP_NAME_OF_TYPE(EntityId))
        .constructor()
        .property("id", &EntityId::id);
      
      Registration::enumeration<LayerMask>(HYP_NAME_OF_TYPE(LayerMask))(
        Value(HYP_NAME_OF_ENUM(LayerMask::Nothing), LayerMask::Nothing),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer1), LayerMask::Layer1),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer2), LayerMask::Layer2),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer3), LayerMask::Layer3),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer4), LayerMask::Layer4),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer5), LayerMask::Layer5),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer6), LayerMask::Layer6),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer7), LayerMask::Layer7),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer8), LayerMask::Layer8),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer9), LayerMask::Layer9),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer10), LayerMask::Layer10),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer11), LayerMask::Layer11),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer12), LayerMask::Layer12),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer13), LayerMask::Layer13),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer14), LayerMask::Layer14),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer15), LayerMask::Layer15),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer16), LayerMask::Layer16),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer17), LayerMask::Layer17),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer18), LayerMask::Layer18),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer19), LayerMask::Layer19),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer20), LayerMask::Layer20),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer21), LayerMask::Layer21),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer22), LayerMask::Layer22),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer23), LayerMask::Layer23),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer24), LayerMask::Layer24),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer25), LayerMask::Layer25),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer26), LayerMask::Layer26),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer27), LayerMask::Layer27),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer28), LayerMask::Layer28),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer29), LayerMask::Layer29),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer30), LayerMask::Layer30),
        Value(HYP_NAME_OF_ENUM(LayerMask::Layer31), LayerMask::Layer31),
        Value(HYP_NAME_OF_ENUM(LayerMask::Everything), LayerMask::Everything)
      );
      
      Registration::enumeration<CameraProjectionMode>(HYP_NAME_OF_TYPE(CameraProjectionMode))(
       Value(HYP_NAME_OF_ENUM(CameraProjectionMode::Perspective), CameraProjectionMode::Perspective),
       Value(HYP_NAME_OF_ENUM(CameraProjectionMode::Orthographic), CameraProjectionMode::Orthographic)
      );

      Registration::enumeration<CameraClearMode>(HYP_NAME_OF_TYPE(CameraClearMode))(
       Value(HYP_NAME_OF_ENUM(CameraClearMode::Nothing), CameraClearMode::Nothing),
       Value(HYP_NAME_OF_ENUM(CameraClearMode::Depth), CameraClearMode::Depth),
       Value(HYP_NAME_OF_ENUM(CameraClearMode::Color), CameraClearMode::Color),
       Value(HYP_NAME_OF_ENUM(CameraClearMode::Skybox), CameraClearMode::Skybox)
      );

      Registration::enumeration<ScalingMode>(HYP_NAME_OF_TYPE(ScalingMode))(
       Value(HYP_NAME_OF_ENUM(ScalingMode::ScaleWithScreenSize), ScalingMode::ScaleWithScreenSize),
       Value(HYP_NAME_OF_ENUM(ScalingMode::ConstantPixelSize), ScalingMode::ConstantPixelSize)
      );

      Registration::class_<CameraViewportClipping>(HYP_NAME_OF_TYPE(CameraViewportClipping))
        .constructor<>()
        .property("x", &CameraViewportClipping::x)
        .property("y", &CameraViewportClipping::y)
        .property("width", &CameraViewportClipping::width)
        .property("height", &CameraViewportClipping::height);
    }
    
    // Core.
    {
      Registration::class_<NameComponent>(HYP_NAME_OF_TYPE(NameComponent))
        .constructor<>()
        .property("name", &NameComponent::name);

      Registration::class_<TagComponent>(HYP_NAME_OF_TYPE(TagComponent))
        .constructor<>()
        .property("tag", &TagComponent::tag);

      Registration::class_<DisabledComponent>(HYP_NAME_OF_TYPE(DisabledComponent))
        .constructor<>();

      Registration::class_<StaticComponent>(HYP_NAME_OF_TYPE(StaticComponent))
        .constructor<>();

      Registration::class_<ArchetypeComponent>(HYP_NAME_OF_TYPE(ArchetypeComponent))
        .constructor<>();
    }

    // Transform.
    {
      Registration::class_<LocalTransformComponent>(HYP_NAME_OF_TYPE(LocalTransformComponent))
        (Metadata(TypeMetadata::EditorName, "TransformComponent"))
        .constructor<>()
        .property("position", &LocalTransformComponent::position)
        .property("rotation", &LocalTransformComponent::rotation)
        .property("scale", &LocalTransformComponent::scale);
      
      Registration::class_<DerivedTransformComponent>(HYP_NAME_OF_TYPE(DerivedTransformComponent))
        (Metadata(TypeMetadata::HideInEditor, true))
        .constructor<>()
        .property("position", &DerivedTransformComponent::position)(Metadata(PropertyMetadata::Serialize, false))
        .property("rotation", &DerivedTransformComponent::rotation)(Metadata(PropertyMetadata::Serialize, false))
        .property("scale", &DerivedTransformComponent::scale)(Metadata(PropertyMetadata::Serialize, false));

      Registration::class_<LocalToWorldComponent>(HYP_NAME_OF_TYPE(LocalToWorldComponent))
        (Metadata(TypeMetadata::HideInEditor, true))
        .constructor<>()
        .property("local_to_world", &LocalToWorldComponent::local_to_world)(Metadata(PropertyMetadata::Serialize, false));

      Registration::class_<HierarchyComponent>(HYP_NAME_OF_TYPE(HierarchyComponent))
        (Metadata(TypeMetadata::HideInEditor, true))
        .constructor<>()
        .property("parent", &HierarchyComponent::parent)
        .property("previous_sibling", &HierarchyComponent::previous_sibling)
        .property("next_sibling", &HierarchyComponent::next_sibling)
        .property("child_count", &HierarchyComponent::child_count)
        .property("first_child", &HierarchyComponent::first_child)
        .property("last_child", &HierarchyComponent::last_child);
    }
    
    // Physics.
    {
      using namespace Physics;
      
      Registration::class_<BoxColliderComponent>(HYP_NAME_OF_TYPE(BoxColliderComponent))
        .constructor<>()
        .property("origin", &BoxColliderComponent::origin)
        .property("size", &BoxColliderComponent::size);

      Registration::class_<SphereColliderComponent>(HYP_NAME_OF_TYPE(SphereColliderComponent))
        .constructor<>()
        .property("radius", &SphereColliderComponent::radius);
    }

    // Render.
    {
      using namespace Rendering;
      
      Registration::class_<CameraComponent>(HYP_NAME_OF_TYPE(CameraComponent))
        .constructor<>()
        .property("projection_mode", &CameraComponent::projection_mode)
        .property("clear_mode", &CameraComponent::clear_mode)
        .property("background_color", &CameraComponent::background_color)
        .property("culling_mask", &CameraComponent::culling_mask)
        .property("near_plane", &CameraComponent::near_plane)
        .property("far_plane", &CameraComponent::far_plane)
        .property("fov", &CameraComponent::fov)
        .property("orthographic_size", &CameraComponent::orthographic_size)
        .property("viewport_clipping", &CameraComponent::viewport_clipping)
        .property("view_matrix", &CameraComponent::view_matrix)(Metadata(PropertyMetadata::Serialize, false))
        .property("projection_matrix", &CameraComponent::projection_matrix)(Metadata(PropertyMetadata::Serialize, false))
        .property("view_projection_matrix", &CameraComponent::view_projection_matrix)(Metadata(PropertyMetadata::Serialize, false))
        .property("inverse_view_matrix", &CameraComponent::inverse_view_matrix)(Metadata(PropertyMetadata::Serialize, false))
        .property("inverse_projection_matrix", &CameraComponent::inverse_projection_matrix)(Metadata(PropertyMetadata::Serialize, false))
        .property("inverse_view_projection_matrix", &CameraComponent::inverse_view_projection_matrix)(Metadata(PropertyMetadata::Serialize, false));

      Registration::class_<SpriteComponent>(HYP_NAME_OF_TYPE(SpriteComponent))
        .constructor<>()
        .property("color", &SpriteComponent::color);

      Registration::class_<MeshComponent>(HYP_NAME_OF_TYPE(MeshComponent))
        .constructor<>()
        .property("mesh", &MeshComponent::mesh)
        .property("sub_mesh_index", &MeshComponent::sub_mesh_index)
        .property("material", &MeshComponent::material)
        .property("layer_mask", &MeshComponent::layer_mask);

      Registration::class_<MeshBoundsComponent>(HYP_NAME_OF_TYPE(MeshBoundsComponent))
        (Metadata(TypeMetadata::HideInEditor, true))
        .constructor<>()
        .property("bounds", &MeshBoundsComponent::bounds)(Metadata(PropertyMetadata::Serialize, false));

      Registration::class_<LightComponent>(HYP_NAME_OF_TYPE(LightComponent))
        .property("intensity", &LightComponent::intensity)
        .property("color", &LightComponent::color);

      Registration::class_<DirectionalLightComponent>(HYP_NAME_OF_TYPE(DirectionalLightComponent))
        .constructor<>();

      Registration::class_<PointLightComponent>(HYP_NAME_OF_TYPE(PointLightComponent))
        .constructor<>()
        .property("range", &PointLightComponent::range);

      Registration::class_<SpotLightComponent>(HYP_NAME_OF_TYPE(SpotLightComponent))
        .constructor<>()
        .property("inner_spot_radius", &SpotLightComponent::inner_spot_radius)
        .property("outer_spot_radius", &SpotLightComponent::outer_spot_radius);
    }

    // UI.
    {
      using namespace UI;
      
      Registration::class_<UIViewComponent>(HYP_NAME_OF_TYPE(UIViewComponent))
        .constructor<>()
        .property("scaling_mode", &UIViewComponent::scaling_mode)
        .property("reference_resolution", &UIViewComponent::reference_resolution);
    }

    RegisterCoreComponents();
  }
  
  //--------------------------------------------------------------
  void ComponentRegistry::RegisterCoreComponents() {
    // Core.
    RegisterComponent<NameComponent>();
    RegisterComponent<TagComponent>();
    RegisterComponent<DisabledComponent>();
    RegisterComponent<StaticComponent>();
    RegisterComponent<ArchetypeComponent>();

    // Transform.
    RegisterComponent<LocalTransformComponent>();
    RegisterComponent<DerivedTransformComponent>();
    RegisterComponent<LocalToWorldComponent>();
    RegisterComponent<HierarchyComponent>();

    // Physics.
    RegisterComponent<Physics::BoxColliderComponent>();
    RegisterComponent<Physics::SphereColliderComponent>();

    // Render.
    RegisterComponent<Rendering::CameraComponent>();
    RegisterComponent<Rendering::SpriteComponent>();
    RegisterComponent<Rendering::MeshComponent>();
    RegisterComponent<Rendering::MeshBoundsComponent>();
    RegisterComponent<Rendering::DirectionalLightComponent>();
    RegisterComponent<Rendering::PointLightComponent>();
    RegisterComponent<Rendering::SpotLightComponent>();

    // UI.
    RegisterComponent<UI::UIViewComponent>();
    
    // TODO: When should user-defined components be registered?
  }

}
