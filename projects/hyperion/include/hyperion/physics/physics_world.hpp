#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/entity/entity_types.hpp"
#include "hyperion/ecs/component/components/physics_components.hpp"
#include "hyperion/ecs/component/components/transform_components.hpp"
#include "hyperion/physics/physics_types.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class World;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Physics {

    class IPhysicsWorld {
    public:
        virtual ~IPhysicsWorld() = default;
    public:
        virtual bool Raycast(Ray ray, RaycastResult &result, float32 distance = 1000.0f) = 0;

        virtual void UpdateBoxCollider(World *world, EntityId entity, BoxColliderComponent *box_collider, DerivedTransformComponent *derived_transform) = 0;
        virtual void UpdateBoxColliderTransform(World *world, EntityId entity, BoxColliderComponent *box_collider, DerivedTransformComponent *derived_transform) = 0;
        virtual void UpdateSphereCollider(World *world, EntityId entity, SphereColliderComponent *sphere_collider) = 0;
        virtual void UpdateSphereColliderTransform(World *world, EntityId entity, SphereColliderComponent *sphere_collider, DerivedTransformComponent *derived_transform) = 0;
    };

}