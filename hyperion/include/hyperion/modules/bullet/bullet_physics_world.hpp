//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <bullet/btBulletCollisionCommon.h>

//---------------------- Project Includes ----------------------
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
    ~BulletPhysicsWorld() override;

    bool8 Raycast(Ray ray, RaycastResult &result, float32 distance) override;

    void UpdateBoxCollider(EntityManager *manager, EntityId entity, BoxColliderComponent *box_collider, DerivedTransformComponent *derived_transform) override;
    void UpdateBoxColliderTransform(EntityManager *manager, EntityId entity, BoxColliderComponent *box_collider,
                                    DerivedTransformComponent *derived_transform) override;
    void UpdateSphereCollider(EntityManager *manager, EntityId entity, SphereColliderComponent *sphere_collider) override;
    void UpdateSphereColliderTransform(EntityManager *manager, EntityId entity, SphereColliderComponent *sphere_collider,
                                       DerivedTransformComponent *derived_transform) override;
  private:
    void AddBoxCollider(EntityManager *manager, EntityId entity);
    void RemoveBoxCollider(EntityManager *manager, EntityId entity);
    void AddSphereCollider(EntityManager *manager, EntityId entity);
    void RemoveSphereCollider(EntityManager *manager, EntityId entity);

    void AddCollider(ColliderStorage &collider_storage, EntityId entity, btCollisionObject *collision_object);
    void RemoveCollider(ColliderStorage &collider_storage, EntityId entity);
    void UpdateTransform(DerivedTransformComponent *transform, btCollisionObject *collision_object, Vector3 position);
  private:
    btCollisionWorld *m_collision_world;
    btDispatcher *m_collision_dispatcher;
    btBroadphaseInterface *m_broadphase;

    ColliderStorage m_box_colliders;
    ColliderStorage m_sphere_colliders;
  };

}
