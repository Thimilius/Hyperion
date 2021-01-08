#include "hyppch.hpp"

#include "hyperion/modules/bullet/bullet_physics_world.hpp"

#include "hyperion/modules/bullet/bullet_physics_driver.hpp"
#include "hyperion/modules/bullet/bullet_physics_debug_drawer.hpp"
#include "hyperion/entity/components/transform.hpp"
#include "hyperion/entity/components/physics/box_collider.hpp"
#include "hyperion/entity/components/physics/sphere_collider.hpp"

namespace Hyperion::Physics {

    BulletPhysicsWorld::BulletPhysicsWorld(BulletPhysicsDriver *driver) {
        btCollisionConfiguration *collision_configuration = driver->m_collision_configuration;
        m_collision_world = new btCollisionWorld(new btCollisionDispatcher(collision_configuration), new btDbvtBroadphase(), collision_configuration);
        m_collision_world->setDebugDrawer(new BulletDebugDrawer());
    }

    BulletPhysicsWorld::~BulletPhysicsWorld() {
        for (auto [collider, collision_object] : m_collision_objects) {
            m_collision_world->removeCollisionObject(collision_object);
            delete collision_object->getCollisionShape();
            delete collision_object;
        }

        delete m_collision_world->getBroadphase();
        delete m_collision_world->getDispatcher();
        delete m_collision_world;
    }

    bool BulletPhysicsWorld::Raycast(Ray ray, RaycastResult &result, f32 distance) {
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
            result.collider = static_cast<Collider *>(result_callback.m_collisionObject->getUserPointer());
            result.point = Vec3(point.x(), point.y(), point.z());
            result.normal = Vec3(normal.x(), normal.y(), normal.z());
            return true;
        } else {
            return false;
        }
    }

    void BulletPhysicsWorld::DebugDraw() {
        m_collision_world->debugDrawWorld();
    }

    void BulletPhysicsWorld::AddBoxCollider(BoxCollider *box_collider) {
        btCollisionObject *collision_object = new btCollisionObject();
        Vec3 half_extends = 0.5f * box_collider->GetSize();
        collision_object->setCollisionShape(new btBoxShape(btVector3(half_extends.x, half_extends.y, half_extends.z)));

        Transform *transform = box_collider->GetTransform();
        Vec3 position = transform->LocalToWorldPosition(box_collider->GetOrigin());
        UpdateTransform(transform, collision_object, position);

        AddCollider(box_collider, collision_object);
    }

    void BulletPhysicsWorld::AddSphereCollider(SphereCollider *sphere_collider) {
        btCollisionObject *collision_object = new btCollisionObject();
        collision_object->setCollisionShape(new btSphereShape(sphere_collider->GetRadius()));

        Transform *transform = sphere_collider->GetTransform();
        UpdateTransform(transform, collision_object, transform->GetPosition());

        AddCollider(sphere_collider, collision_object);
    }

    void BulletPhysicsWorld::RemoveCollider(Collider *collider) {
        btCollisionObject *collision_object = m_collision_objects.at(collider);
        m_collision_objects.erase(collider);

        m_collision_world->removeCollisionObject(collision_object);

        // We need to clear the mesh of a mesh collider
        if (collider->GetColliderType() == ColliderType::MeshCollider) {
            btBvhTriangleMeshShape *mesh_shape = static_cast<btBvhTriangleMeshShape *>(collision_object->getCollisionShape());
            delete mesh_shape->getMeshInterface();
        }

        delete collision_object->getCollisionShape();
        delete collision_object;
    }

    void BulletPhysicsWorld::UpdateBoxCollider(BoxCollider *box_collider) {
        btCollisionObject *collision_object = m_collision_objects.at(box_collider);

        delete collision_object->getCollisionShape();

        Vec3 half_extends = 0.5f * box_collider->GetSize();
        Vec3 scale = box_collider->GetTransform()->GetScale();
        btBoxShape *collision_box = new btBoxShape(btVector3(half_extends.x, half_extends.y, half_extends.z));
        collision_box->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
        collision_object->setCollisionShape(collision_box);

        Transform *transform = box_collider->GetTransform();
        Vec3 position = transform->LocalToWorldPosition(box_collider->GetOrigin());
        collision_object->getWorldTransform().setOrigin(btVector3(position.x, position.y, position.z));

        m_collision_world->updateSingleAabb(collision_object);
    }

    void BulletPhysicsWorld::UpdateSphereCollider(SphereCollider *sphere_collider) {
        btCollisionObject *collision_object = m_collision_objects.at(sphere_collider);

        btSphereShape *collision_sphere = static_cast<btSphereShape *>(collision_object->getCollisionShape());
        collision_sphere->setUnscaledRadius(sphere_collider->GetRadius());

        m_collision_world->updateSingleAabb(collision_object);
    }

    void BulletPhysicsWorld::UpdateBoxColliderTransform(BoxCollider *box_collider) {
        btCollisionObject *collision_object = m_collision_objects.at(box_collider);

        // The box collider needs special transformation based on its origin
        Transform *transform = box_collider->GetTransform();
        Vec3 position = transform->LocalToWorldPosition(box_collider->GetOrigin());
        UpdateTransform(transform, collision_object, position);

        m_collision_world->updateSingleAabb(collision_object);
    }

    void BulletPhysicsWorld::UpdateSphereColliderTransform(SphereCollider *sphere_collider) {
        btCollisionObject *collision_object = m_collision_objects.at(sphere_collider);

        Transform *transform = sphere_collider->GetTransform();
        UpdateTransform(transform, collision_object, transform->GetPosition());

        m_collision_world->updateSingleAabb(collision_object);
    }

    void BulletPhysicsWorld::UpdateColliderActivation(Collider *collider) {
        btCollisionObject *collision_object = m_collision_objects.at(collider);
        if (collider->IsActiveAndEnabled()) {
            m_collision_world->addCollisionObject(collision_object);

            // When reactivating the collider we need it to update to its current state
            // FIXME: This is probably not the most elegant solution
            switch (collider->GetColliderType()) {
                case ColliderType::BoxCollider: {
                    BoxCollider *box_collider = static_cast<BoxCollider *>(collider);
                    UpdateBoxCollider(box_collider);
                    UpdateBoxColliderTransform(box_collider);
                    break;
                }
                case ColliderType::SphereCollider: {
                    SphereCollider *sphere_collider = static_cast<SphereCollider *>(collider);
                    UpdateSphereCollider(sphere_collider);
                    UpdateSphereColliderTransform(sphere_collider);
                    break;
                }
                default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
            }
        } else {
            m_collision_world->removeCollisionObject(collision_object);
        }
    }

    BoundingBox BulletPhysicsWorld::GetBounds(Collider *collider) {
        auto collision_object_pos = m_collision_objects.find(collider);
        if (collision_object_pos != m_collision_objects.end()) {
            btCollisionObject *collision_object = collision_object_pos->second;

            if (collider->IsActiveAndEnabled()) {
                btVector3 min;
                btVector3 max;
                collision_object->getCollisionShape()->getAabb(collision_object->getWorldTransform(), min, max);
                return BoundingBox(Vec3(min.x(), min.y(), min.z()), Vec3(max.x(), max.y(), max.z()));
            } else {
                // NOTE: Do we really want to return empty bounds when the collider is not active?
                return BoundingBox();
            }
        } else {
            return BoundingBox();
        }
    }

    void BulletPhysicsWorld::AddCollider(Collider *collider, btCollisionObject *collision_object) {
        collision_object->setUserPointer(collider);
        m_collision_world->addCollisionObject(collision_object);
        m_collision_objects[collider] = collision_object;
    }

    void BulletPhysicsWorld::UpdateTransform(Transform *transform, btCollisionObject *collision_object, Vec3 position) {
        Quaternion rotation = transform->GetRotation();
        Vec3 scale = transform->GetScale();
        collision_object->getWorldTransform().setOrigin(btVector3(position.x, position.y, position.z));
        collision_object->getWorldTransform().setRotation(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w));
        collision_object->getCollisionShape()->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
    }

}