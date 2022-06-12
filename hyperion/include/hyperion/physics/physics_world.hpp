//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/ray.hpp"
#include "hyperion/ecs/entity/entity_types.hpp"
#include "hyperion/ecs/component/components/physics_components.hpp"
#include "hyperion/ecs/component/components/transform_components.hpp"
#include "hyperion/physics/physics_types.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class World;
  class EntityManager;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Physics {

  class IPhysicsWorld {
  public:
    virtual ~IPhysicsWorld() = default;
  public:
    virtual bool8 Raycast(Ray ray, RaycastResult &result, float32 distance = 1000.0f) = 0;

    virtual void UpdateBoxCollider(
      EntityManager *manager,
      EntityId entity,
      BoxColliderComponent *box_collider,
      DerivedTransformComponent *derived_transform) = 0;
    virtual void UpdateBoxColliderTransform(
      EntityManager *manager,
      EntityId entity,
      BoxColliderComponent *box_collider,
      DerivedTransformComponent *derived_transform) = 0;
    virtual void UpdateSphereCollider(EntityManager *manager, EntityId entity, SphereColliderComponent *sphere_collider) = 0;
    virtual void UpdateSphereColliderTransform(
      EntityManager *manager,
      EntityId entity,
      SphereColliderComponent *sphere_collider,
      DerivedTransformComponent *derived_transform) = 0;
  };

  class NullPhysicsWorld final : public IPhysicsWorld {
  public:
    bool8 Raycast(Ray ray, RaycastResult &result, float32 distance = 1000.0f) override { return false; }

    void UpdateBoxCollider(
      EntityManager *manager,
      EntityId entity,
      BoxColliderComponent *box_collider,
      DerivedTransformComponent *derived_transform) override { }
    void UpdateBoxColliderTransform(
      EntityManager *manager,
      EntityId entity,
      BoxColliderComponent *box_collider,
      DerivedTransformComponent *derived_transform) override { }
    void UpdateSphereCollider(EntityManager *manager, EntityId entity, SphereColliderComponent *sphere_collider) override { }
    void UpdateSphereColliderTransform(
      EntityManager *manager,
      EntityId entity,
      SphereColliderComponent *sphere_collider,
      DerivedTransformComponent *derived_transform) override { }
  };

}
