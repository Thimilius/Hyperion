//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/bullet/bullet_physics_driver.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/world_manager.hpp"
#include "hyperion/modules/bullet/bullet_physics_world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Physics {

    //--------------------------------------------------------------
    PhysicsWorld *BulletPhysicsDriver::CreatePhysicsWorld() {
        return new BulletPhysicsWorld(this);
    }

    //--------------------------------------------------------------
    void BulletPhysicsDriver::DestroyPhysicsWorld(PhysicsWorld *world) {
        delete world;
    }

    //--------------------------------------------------------------
    void BulletPhysicsDriver::Initialize() {
        m_collision_configuration = new btDefaultCollisionConfiguration();

        HYP_LOG_INFO("Physics", "Initialized Bullet physics driver!");
    }

    //--------------------------------------------------------------
    void BulletPhysicsDriver::FixedUpdate(float32 delta_time) {

    }

    //--------------------------------------------------------------
    void BulletPhysicsDriver::Shutdown() {
        delete m_collision_dispatcher;
        delete m_collision_configuration;
    }

}