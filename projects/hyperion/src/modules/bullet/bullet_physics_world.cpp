#include "hyppch.hpp"

#include "hyperion/modules/bullet/bullet_physics_world.hpp"

#include "hyperion/modules/bullet/bullet_physics_driver.hpp"

namespace Hyperion::Physics {

    BulletPhysicsWorld::BulletPhysicsWorld(BulletPhysicsDriver *driver) {
        m_collision_world = new btCollisionWorld(driver->m_collision_dispatcher, driver->m_collision_broadphase_interface, driver->m_collision_configuration);
    }

    BulletPhysicsWorld::~BulletPhysicsWorld() {
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
            result.point = Vec3(point.x(), point.y(), point.z());
            result.normal = Vec3(normal.x(), normal.y(), normal.z());
            return true;
        } else {
            return false;
        }
    }

}