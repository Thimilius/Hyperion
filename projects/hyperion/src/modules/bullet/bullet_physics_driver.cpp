#include "hyppch.hpp"

#include "hyperion/modules/bullet/bullet_physics_driver.hpp"

#include "hyperion/modules/bullet/bullet_physics_world.hpp"
#include "hyperion/entity/world_manager.hpp"

namespace Hyperion::Physics {

    bool BulletPhysicsDriver::Raycast(Ray ray, RaycastResult &result, f32 distance) {
        return WorldManager::GetActiveWorld()->GetPhysicsWorld()->Raycast(ray, result, distance);
    }

    PhysicsWorld *BulletPhysicsDriver::CreatePhysicsWorld() {
        return new BulletPhysicsWorld(this);
    }

    void BulletPhysicsDriver::DestroyPhysicsWorld(PhysicsWorld *world) {
        delete world;
    }

    void BulletPhysicsDriver::Init() {
        m_collision_configuration = new btDefaultCollisionConfiguration();

        HYP_LOG_INFO("Physics", "Initialized Bullet physics engine!");
    }

    void BulletPhysicsDriver::Update(f32 delta_time) {

    }

    void BulletPhysicsDriver::Shutdown() {
        delete m_collision_dispatcher;
        delete m_collision_configuration;
    }

}