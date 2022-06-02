//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/system/systems/physics_systems.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/components/core_components.hpp"
#include "hyperion/ecs/component/components/transform_components.hpp"
#include "hyperion/ecs/world/world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Physics {

  //--------------------------------------------------------------
  void UpdateColliderTransformSystem::Run(EntityManager *manager) {
    HYP_PROFILE_SCOPE("UpdateColliderTransformSystem.Run");
    
    IPhysicsWorld *physics_world = manager->GetWorld()->GetPhysicsWorld();
    
    auto box_collider_view = manager->GetViewAll<DerivedTransformComponent, BoxColliderComponent>(ExcludeComponents<StaticComponent>());
    for (EntityId entity : box_collider_view) {
      DerivedTransformComponent *derived_transform = manager->GetComponent<DerivedTransformComponent>(entity);
      BoxColliderComponent *box_collider = manager->GetComponent<BoxColliderComponent>(entity);
      physics_world->UpdateBoxColliderTransform(manager, entity, box_collider, derived_transform);
    }

    auto sphere_collider_view = manager->GetView<DerivedTransformComponent, SphereColliderComponent>(ExcludeComponents<StaticComponent>());
    for (EntityId entity : sphere_collider_view) {
      DerivedTransformComponent *derived_transform = manager->GetComponent<DerivedTransformComponent>(entity);
      SphereColliderComponent *sphere_collider = manager->GetComponent<SphereColliderComponent>(entity);
      physics_world->UpdateSphereColliderTransform(manager, entity, sphere_collider, derived_transform);
    }
  }

}
