#include "hyppch.hpp"

#include "hyperion/modules/bullet/bullet_physics_world.hpp"

#include "hyperion/modules/bullet/bullet_physics_driver.hpp"
#include "hyperion/entity/components/transform.hpp"
#include "hyperion/entity/components/physics/box_collider.hpp"
#include "hyperion/entity/components/physics/sphere_collider.hpp"

namespace Hyperion::Physics {

    BulletPhysicsWorld::BulletPhysicsWorld(BulletPhysicsDriver *driver) {
        m_collision_world = new btCollisionWorld(driver->m_collision_dispatcher, driver->m_collision_broadphase_interface, driver->m_collision_configuration);
    }

    BulletPhysicsWorld::~BulletPhysicsWorld() {
        for (auto pair : m_collision_objects) {
            delete pair.second->getCollisionShape();
            delete pair.second;
        }

        delete m_collision_world;
    }

    bool BulletPhysicsWorld::Raycast(Ray ray, RaycastResult &result, f32 distance) {
        btVector3 from = btVector3(ray.origin.x, ray.origin.y, ray.origin.z);
        btVector3 to = from + (distance * btVector3(ray.direction.x, ray.direction.y, ray.direction.z));

        btCollisionWorld::ClosestRayResultCallback result_callback(from, to);
        m_collision_world->rayTest(from, to, result_callback);

        if (result_callback.hasHit()) {
            btVector3 point = result_callback.m_hitPointWorld;
            btVector3 normal = result_callback.m_hitNormalWorld;
            result.collider = static_cast<Collider *>(result_callback.m_collisionObject->getUserPointer());
            result.point = Vec3(point.x(), point.y(), point.z());
            result.normal = Vec3(normal.x(), normal.y(), normal.z());
            return true;
        } else {
            return false;
        }
    }

    void BulletPhysicsWorld::AddBoxCollider(BoxCollider *box_collider) {
        btCollisionObject *collision_object = new btCollisionObject();
        collision_object->setUserPointer(box_collider);
        Vec3 half_extends = 0.5f * box_collider->GetSize();
        collision_object->setCollisionShape(new btBoxShape(btVector3(half_extends.x, half_extends.y, half_extends.z)));

        UpdateTransform(box_collider, collision_object);

        AddCollider(box_collider, collision_object);
    }

    void BulletPhysicsWorld::AddSphereCollider(SphereCollider *sphere_collider) {
        btCollisionObject *collision_object = new btCollisionObject();
        collision_object->setUserPointer(sphere_collider);
        collision_object->setCollisionShape(new btSphereShape(sphere_collider->GetRadius()));

        UpdateTransform(sphere_collider, collision_object);

        AddCollider(sphere_collider, collision_object);
    }

    void BulletPhysicsWorld::RemoveCollider(Collider *collider) {
        btCollisionObject *collision_object = m_collision_objects.at(collider);
        m_collision_objects.erase(collider);

        m_collision_world->removeCollisionObject(collision_object);

        delete collision_object->getCollisionShape();
        delete collision_object;
    }

    void BulletPhysicsWorld::UpdateColliderTransform(Collider *collider) {

    }

    void BulletPhysicsWorld::AddCollider(Collider *collider, btCollisionObject *collision_object) {
        m_collision_world->addCollisionObject(collision_object);
        m_collision_objects[collider] = collision_object;
    }

    void BulletPhysicsWorld::UpdateTransform(Collider *collider, btCollisionObject *collision_object) {
        Transform *transform = collider->GetTransform();
        Vec3 position = transform->GetPosition();
        Quaternion rotation = transform->GetRotation();
        Vec3 scale = transform->GetScale();
        collision_object->getWorldTransform().setOrigin(btVector3(position.x, position.y, position.z));
        collision_object->getWorldTransform().setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
        collision_object->getCollisionShape()->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
    }

}