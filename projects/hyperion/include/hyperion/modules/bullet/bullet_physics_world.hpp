#pragma once

#include "hyperion/physics/physics_world.hpp"

#include <bullet/btBulletCollisionCommon.h>

namespace Hyperion::Physics {

    class BulletPhysicsDriver;

    class BulletPhysicsWorld : public PhysicsWorld {
    private:
        btCollisionWorld *m_collision_world;
        Map<Collider *, btCollisionObject *> m_collision_objects;
    public:
        BulletPhysicsWorld(BulletPhysicsDriver *driver);
        ~BulletPhysicsWorld() override;

        bool Raycast(Ray ray, RaycastResult &result, f32 distance);
    private:
        void AddBoxCollider(BoxCollider *box_collider) override;
        void AddSphereCollider(SphereCollider *sphere_collider) override;

        void RemoveCollider(Collider *collider) override;

        void UpdateBoxCollider(BoxCollider *box_collider) override;
        void UpdateSphereCollider(SphereCollider *sphere_collider) override;
        void UpdateColliderTransform(Collider *collider) override;
        void UpdateColliderActivation(Collider *collider) override;

        void AddCollider(Collider *collider, btCollisionObject *collision_object);
        void UpdateTransform(Collider *collider, btCollisionObject *collision_object);
    };

}