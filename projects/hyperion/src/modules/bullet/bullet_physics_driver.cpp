#include "hyppch.hpp"

#include "hyperion/modules/bullet/bullet_physics_driver.hpp"

#include "hyperion/editor/editor_engine.hpp"

namespace Hyperion::Physics {

    // TODO: We need some sort of physics world abstraction instead of a global physics representation

    void BulletPhysicsDriver::Init() {
        m_collision_configuration = new btDefaultCollisionConfiguration();
        m_collision_dispatcher = new btCollisionDispatcher(m_collision_configuration);
        m_collision_broadphase_interface = new btDbvtBroadphase();

        m_collision_world = new btCollisionWorld(m_collision_dispatcher, m_collision_broadphase_interface, m_collision_configuration);

        btCollisionObject *obj = new btCollisionObject();
        obj->setCollisionShape(new btSphereShape(0.5f));
        m_collision_world->addCollisionObject(obj);
        
        HYP_LOG_INFO("Physics", "Initialized Bullet physics engine!");
    }

    void BulletPhysicsDriver::Update(f32 delta_time) {
        Ray ray = Editor::EditorEngine::GetCamera()->ScreenPointToRay(Input::GetMousePosition());
        btVector3 from = btVector3(ray.origin.x, ray.origin.y, ray.origin.z);
        btVector3 to = from + (100 * btVector3(ray.direction.x, ray.direction.y, ray.direction.z));
        btCollisionWorld::ClosestRayResultCallback result(from, to);
        m_collision_world->rayTest(from, to, result);
        if (result.hasHit()) {
            Vec3 hit = Vec3(result.m_hitPointWorld.x(), result.m_hitPointWorld.y(), result.m_hitPointWorld.z());
            HYP_TRACE("We hit! {}", hit.ToString());
        }
    }

    void BulletPhysicsDriver::Shutdown() {
        delete m_collision_world;
        delete m_collision_broadphase_interface;
        delete m_collision_dispatcher;
        delete m_collision_configuration;
    }

}