//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/bullet/bullet_physics_world.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/ecs/world/world.hpp"
#include "hyperion/ecs/component/components/utilities/transform_utilities.hpp"
#include "hyperion/modules/bullet/bullet_physics_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Physics {

  //--------------------------------------------------------------
  BulletPhysicsWorld::BulletPhysicsWorld(World *world, BulletPhysicsDriver *driver) : BulletPhysicsWorld(world, driver, nullptr) { }

  //--------------------------------------------------------------
  BulletPhysicsWorld::BulletPhysicsWorld(World *world, BulletPhysicsDriver *driver, IPhysicsWorld *other) {
    btCollisionConfiguration *collision_configuration = driver->GetCollisionConfiguration();
    m_collision_dispatcher = new btCollisionDispatcher(collision_configuration);
    m_broadphase = new btDbvtBroadphase();
    m_collision_world = new btCollisionWorld(m_collision_dispatcher, m_broadphase, collision_configuration);

    EntityManager *manager = world->GetEntityManager();
    manager->RegisterOnComponentAdded<BoxColliderComponent>({ ConnectionArguments<&BulletPhysicsWorld::AddBoxCollider>, this });
    manager->RegisterOnComponentRemoved<BoxColliderComponent>({ ConnectionArguments<&BulletPhysicsWorld::RemoveBoxCollider>, this });
    manager->RegisterOnComponentAdded<SphereColliderComponent>({ ConnectionArguments<&BulletPhysicsWorld::AddSphereCollider>, this });
    manager->RegisterOnComponentRemoved<SphereColliderComponent>({ ConnectionArguments<&BulletPhysicsWorld::RemoveSphereCollider>, this });

    if (other) {
      CopyColliders(world);
    }
  }

  //--------------------------------------------------------------
  BulletPhysicsWorld::~BulletPhysicsWorld() {
    ClearColliders();
    
    delete m_collision_world;
    delete m_broadphase;
    delete m_collision_dispatcher;
  }

  //--------------------------------------------------------------
  bool8 BulletPhysicsWorld::Raycast(Ray ray, RaycastResult &result, float32 distance) {
    HYP_PROFILE_SCOPE("BulletPhysicsWorld.Raycast");

    btVector3 from = btVector3(ray.origin.x, ray.origin.y, ray.origin.z);
    btVector3 to = from + (distance * btVector3(ray.direction.x, ray.direction.y, ray.direction.z));

    if (from.isZero() && to.isZero()) {
      return false;
    }

    btCollisionWorld::ClosestRayResultCallback result_callback(from, to);
    m_collision_world->rayTest(from, to, result_callback);

    if (result_callback.hasHit()) {
      btVector3 point = result_callback.m_hitPointWorld;
      btVector3 normal = result_callback.m_hitNormalWorld;
      result.point = Vector3(point.x(), point.y(), point.z());
      result.normal = Vector3(normal.x(), normal.y(), normal.z());
      result.entity = reinterpret_cast<EntityIdType>(result_callback.m_collisionObject->getUserPointer());
      return true;
    } else {
      return false;
    }
  }

  //--------------------------------------------------------------
  void BulletPhysicsWorld::UpdateBoxCollider(EntityManager *manager, EntityId entity, BoxColliderComponent *box_collider, DerivedTransformComponent *derived_transform) {
    HYP_PROFILE_SCOPE("BulletPhysicsWorld.UpdateBoxCollider");

    btCollisionObject *collision_object = m_box_colliders.Get(entity);

    delete collision_object->getCollisionShape();

    Vector3 half_extends = 0.5f * box_collider->size;
    Vector3 scale = derived_transform->scale;
    btBoxShape *collision_box = new btBoxShape(btVector3(half_extends.x, half_extends.y, half_extends.z));
    collision_box->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
    collision_object->setCollisionShape(collision_box);

    Vector3 position = box_collider->origin + derived_transform->position;
    collision_object->getWorldTransform().setOrigin(btVector3(position.x, position.y, position.z));

    m_collision_world->updateSingleAabb(collision_object);
  }

  //--------------------------------------------------------------
  void BulletPhysicsWorld::UpdateBoxColliderTransform(EntityManager *manager, EntityId entity, BoxColliderComponent *box_collider,
                                                      DerivedTransformComponent *derived_transform) {
    HYP_PROFILE_SCOPE("BulletPhysicsWorld.UpdateBoxColliderTransform");

    btCollisionObject *collision_object = m_box_colliders.Get(entity);

    Vector3 position = box_collider->origin + derived_transform->position;
    UpdateTransform(derived_transform, collision_object, position);

    m_collision_world->updateSingleAabb(collision_object);
  }

  //--------------------------------------------------------------
  void BulletPhysicsWorld::UpdateSphereCollider(EntityManager *manager, EntityId entity, SphereColliderComponent *sphere_collider) {
    HYP_PROFILE_SCOPE("BulletPhysicsWorld.UpdateSphereCollider");

    btCollisionObject *collision_object = m_sphere_colliders.Get(entity);

    btSphereShape *collision_sphere = static_cast<btSphereShape *>(collision_object->getCollisionShape());
    collision_sphere->setUnscaledRadius(sphere_collider->radius);

    m_collision_world->updateSingleAabb(collision_object);
  }

  //--------------------------------------------------------------
  void BulletPhysicsWorld::UpdateSphereColliderTransform(EntityManager *manager, EntityId entity, SphereColliderComponent *sphere_collider,
                                                         DerivedTransformComponent *derived_transform) {
    HYP_PROFILE_SCOPE("BulletPhysicsWorld.UpdateSphereColliderTransform");

    btCollisionObject *collision_object = m_sphere_colliders.Get(entity);

    UpdateTransform(derived_transform, collision_object, derived_transform->position);

    m_collision_world->updateSingleAabb(collision_object);
  }

  //--------------------------------------------------------------
  void BulletPhysicsWorld::AddBoxCollider(EntityManager *manager, EntityId entity) {
    HYP_PROFILE_SCOPE("BulletPhysicsWorld.AddBoxCollider");

    BoxColliderComponent *box_collider = manager->GetComponent<BoxColliderComponent>(entity);
    DerivedTransformComponent *derived_transform = manager->GetComponent<DerivedTransformComponent>(entity);
    HYP_ASSERT(derived_transform);

    btCollisionObject *collision_object = new btCollisionObject();
    Vector3 half_extends = 0.5f * box_collider->size;
    collision_object->setCollisionShape(new btBoxShape(btVector3(half_extends.x, half_extends.y, half_extends.z)));

    Vector3 position = box_collider->origin + derived_transform->position;
    UpdateTransform(derived_transform, collision_object, position);

    AddCollider(m_box_colliders, entity, collision_object);
  }

  //--------------------------------------------------------------
  void BulletPhysicsWorld::RemoveBoxCollider(EntityManager *manager, EntityId entity) {
    HYP_PROFILE_SCOPE("BulletPhysicsWorld.RemoveBoxCollider");

    RemoveCollider(m_box_colliders, entity);
  }

  //--------------------------------------------------------------
  void BulletPhysicsWorld::AddSphereCollider(EntityManager *manager, EntityId entity) {
    HYP_PROFILE_SCOPE("BulletPhysicsWorld.AddSphereCollider");

    SphereColliderComponent *sphere_collider = manager->GetComponent<SphereColliderComponent>(entity);
    DerivedTransformComponent *derived_transform = manager->GetComponent<DerivedTransformComponent>(entity);
    HYP_ASSERT(derived_transform);

    btCollisionObject *collision_object = new btCollisionObject();
    collision_object->setCollisionShape(new btSphereShape(sphere_collider->radius));

    UpdateTransform(derived_transform, collision_object, derived_transform->position);

    AddCollider(m_sphere_colliders, entity, collision_object);
  }

  //--------------------------------------------------------------
  void BulletPhysicsWorld::RemoveSphereCollider(EntityManager *manager, EntityId entity) {
    HYP_PROFILE_SCOPE("BulletPhysicsWorld.RemoveSphereCollider");

    RemoveCollider(m_sphere_colliders, entity);
  }

  //--------------------------------------------------------------
  void BulletPhysicsWorld::AddCollider(ColliderStorage &collider_storage, EntityId entity, btCollisionObject *collision_object) {
    collision_object->setUserPointer(reinterpret_cast<void *>(entity.id));
    m_collision_world->addCollisionObject(collision_object);
    collider_storage.Insert(entity, collision_object);
  }

  //--------------------------------------------------------------
  void BulletPhysicsWorld::RemoveCollider(ColliderStorage &collider_storage, EntityId entity) {
    btCollisionObject *collision_object = collider_storage.Get(entity);
    collider_storage.Remove(entity);

    m_collision_world->removeCollisionObject(collision_object);

    delete collision_object->getCollisionShape();
    delete collision_object;
  }

  //--------------------------------------------------------------
  void BulletPhysicsWorld::UpdateTransform(DerivedTransformComponent *derived_transform, btCollisionObject *collision_object, Vector3 position) {
    Quaternion rotation = derived_transform->rotation;
    Vector3 scale = derived_transform->scale;
    collision_object->getWorldTransform().setOrigin(btVector3(position.x, position.y, position.z));
    collision_object->getWorldTransform().setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
    collision_object->getCollisionShape()->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
  }

  //--------------------------------------------------------------
  void BulletPhysicsWorld::CopyColliders(World *world) {
    EntityManager *manager = world->GetEntityManager();
    
    auto box_collider_view = manager->GetView<BoxColliderComponent>();
    for (EntityId entity : box_collider_view) {
      AddBoxCollider(manager, entity);
    }

    auto sphere_collider_view = manager->GetView<SphereColliderComponent>();
    for (EntityId entity : sphere_collider_view) {
      AddSphereCollider(manager, entity);
    }
  }

  //--------------------------------------------------------------
  void BulletPhysicsWorld::ClearColliders() {
    for (auto [entity, box_collider_object] : m_box_colliders) {
      m_collision_world->removeCollisionObject(box_collider_object);
      
      delete box_collider_object->getCollisionShape();
      delete box_collider_object;
    }
    m_box_colliders.Clear();

    for (auto [entity, m_sphere_collider] : m_sphere_colliders) {
      m_collision_world->removeCollisionObject(m_sphere_collider);
      
      delete m_sphere_collider->getCollisionShape();
      delete m_sphere_collider;
    }
    m_sphere_colliders.Clear();
  }

}
