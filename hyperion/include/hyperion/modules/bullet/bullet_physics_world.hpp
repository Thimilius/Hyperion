//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <bullet/btBulletCollisionCommon.h>

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/component/component_types.hpp"
#include "hyperion/physics/physics_world.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
  class World;
  
  namespace Physics {
    class BulletPhysicsDriver;
  }
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Physics {

  using ColliderStorage = Map<EntityId, btCollisionObject *>;

  class BulletPhysicsWorld final : public IPhysicsWorld {
  public:
    BulletPhysicsWorld(World *world, BulletPhysicsDriver *driver);
    BulletPhysicsWorld(World *world, BulletPhysicsDriver *driver, IPhysicsWorld *other);
    ~BulletPhysicsWorld() override;

    bool8 Raycast(Ray ray, RaycastResult &result, float32 distance) override;

    void UpdateBoxCollider(EntityManager *manager, EntityId entity, BoxColliderComponent *box_collider, DerivedTransformComponent *derived_transform) override;
    void UpdateBoxColliderTransform(
      EntityManager *manager,
      EntityId entity,
      BoxColliderComponent *box_collider,
      DerivedTransformComponent *derived_transform) override;
    void UpdateSphereCollider(EntityManager *manager, EntityId entity, SphereColliderComponent *sphere_collider) override;
    void UpdateSphereColliderTransform(
      EntityManager *manager,
      EntityId entity,
      SphereColliderComponent *sphere_collider,
      DerivedTransformComponent *derived_transform) override;
  private:
    void AddBoxCollider(EntityManager *manager, ComponentId component_id,  EntityId entity);
    void RemoveBoxCollider(EntityManager *manager, ComponentId component_id, EntityId entity);
    void AddSphereCollider(EntityManager *manager, ComponentId component_id, EntityId entity);
    void RemoveSphereCollider(EntityManager *manager, ComponentId component_id, EntityId entity);

    void AddCollider(ColliderStorage &collider_storage, EntityId entity, btCollisionObject *collision_object);
    void RemoveCollider(ColliderStorage &collider_storage, EntityId entity);
    void UpdateTransform(DerivedTransformComponent *derived_transform, btCollisionObject *collision_object, Vector3 position);

    void CopyColliders(World *world);
    void ClearColliders();
  private:
    World *m_world = nullptr;
    
    btCollisionWorld *m_collision_world = nullptr;
    btDispatcher *m_collision_dispatcher = nullptr;
    btBroadphaseInterface *m_broadphase = nullptr;

    ColliderStorage m_box_colliders;
    ColliderStorage m_sphere_colliders;
  };

}
