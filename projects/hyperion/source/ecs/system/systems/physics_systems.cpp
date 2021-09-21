//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/ecs/system/systems/physics_systems.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/components/transform_components.hpp"
#include "hyperion/ecs/world/world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Physics {

    //--------------------------------------------------------------
    void UpdateColliderTransformSystem::Run(World *world) {
        auto box_collider_view = world->GetView<DerivedTransformComponent, BoxColliderComponent>();
        for (EntityId entity : box_collider_view) {
            DerivedTransformComponent *derived_transform = world->GetComponent<DerivedTransformComponent>(entity);
            BoxColliderComponent *box_collider = world->GetComponent<BoxColliderComponent>(entity);
            world->GetPhysicsWorld()->UpdateBoxColliderTransform(world, entity, box_collider, derived_transform);
        }

        auto sphere_collider_view = world->GetView<DerivedTransformComponent, SphereColliderComponent>();
        for (EntityId entity : sphere_collider_view) {
            DerivedTransformComponent *derived_transform = world->GetComponent<DerivedTransformComponent>(entity);
            SphereColliderComponent *sphere_collider = world->GetComponent<SphereColliderComponent>(entity);
            world->GetPhysicsWorld()->UpdateSphereColliderTransform(world, entity, sphere_collider, derived_transform);
        }
    }

}