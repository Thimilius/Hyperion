#pragma once

//---------------------- Library Includes ----------------------
#include <bullet/btBulletCollisionCommon.h>

//---------------------- Project Includes ----------------------
#include "hyperion/physics/physics_world.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Physics {
    class BulletPhysicsDriver;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Physics {

    using ColliderStorage = Map<EntityId, btCollisionObject *>;

    class BulletPhysicsWorld final : public IPhysicsWorld {
    public:
        BulletPhysicsWorld(World *world, BulletPhysicsDriver *driver);
        ~BulletPhysicsWorld();

        bool Raycast(Ray ray, RaycastResult &result, float32 distance) override;

        void UpdateBoxCollider(World *world, EntityId entity, BoxColliderComponent *box_collider, DerivedTransformComponent *derived_transform) override;
        void UpdateBoxColliderTransform(World *world, EntityId entity, BoxColliderComponent *box_collider, DerivedTransformComponent *derived_transform) override;
        void UpdateSphereCollider(World *world, EntityId entity, SphereColliderComponent *sphere_collider) override;
        void UpdateSphereColliderTransform(World *world, EntityId entity, SphereColliderComponent *sphere_collider, DerivedTransformComponent *derived_transform) override;
    private:
        void AddBoxCollider(World *world, EntityId entity);
        void RemoveBoxCollider(World *world, EntityId entity);
        void AddSphereCollider(World *world, EntityId entity);
        void RemoveSphereCollider(World *world, EntityId entity);

        void AddCollider(ColliderStorage &collider_storage, EntityId entity, btCollisionObject *collision_object);
        void RemoveCollider(ColliderStorage &collider_storage, EntityId entity);
        void UpdateTransform(DerivedTransformComponent *transform, btCollisionObject *collision_object, Vector3 position);
    private:
        btCollisionWorld *m_collision_world;

        ColliderStorage m_box_colliders;
        ColliderStorage m_sphere_colliders;
    };

}