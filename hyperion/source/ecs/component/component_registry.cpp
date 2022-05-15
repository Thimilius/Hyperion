//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/component/component_registry.hpp"
#include "hyperion/ecs/component/components/components.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

  //--------------------------------------------------------------
  void ComponentRegistry::RegisterCoreComponents() {
    // Core.
    RegisterComponent<NameComponent>();
    RegisterComponent<TagComponent>();
    RegisterComponent<DisabledComponent>();
    RegisterComponent<StaticComponent>();
    RegisterComponent<ArchetypeComponent>();

    // Physics.
    RegisterComponent<Physics::BoxColliderComponent>();
    RegisterComponent<Physics::SphereColliderComponent>();

    // Render.
    RegisterComponent<Rendering::CameraComponent>();
    RegisterComponent<Rendering::SpriteComponent>();
    RegisterComponent<Rendering::MeshComponent>();
    RegisterComponent<Rendering::LocalMeshBoundsComponent>();
    RegisterComponent<Rendering::WorldMeshBoundsComponent>();
    RegisterComponent<Rendering::DirectionalLightComponent>();
    RegisterComponent<Rendering::PointLightComponent>();
    RegisterComponent<Rendering::SpotLightComponent>();

    // Transform.
    RegisterComponent<LocalTransformComponent>();
    RegisterComponent<DerivedTransformComponent>();
    RegisterComponent<LocalToWorldComponent>();
    RegisterComponent<HierarchyComponent>();

    // UI.
    RegisterComponent<UI::UIViewComponent>();
  }

}
