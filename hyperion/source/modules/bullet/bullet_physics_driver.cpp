//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/bullet/bullet_physics_driver.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/modules/bullet/bullet_physics_world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Physics {

  //--------------------------------------------------------------
  IPhysicsWorld *BulletPhysicsDriver::CreatePhysicsWorld(World *world) {
    return new BulletPhysicsWorld(world, this);
  }

  //--------------------------------------------------------------
  IPhysicsWorld * BulletPhysicsDriver::CopyPhysicsWorld(World *world, IPhysicsWorld *physics_world) {
    return new BulletPhysicsWorld(world, this, physics_world);
  }

  //--------------------------------------------------------------
  void BulletPhysicsDriver::DestroyPhysicsWorld(IPhysicsWorld *world) {
    delete world;
  }

  //--------------------------------------------------------------
  void BulletPhysicsDriver::Initialize() {
    m_collision_configuration = new btDefaultCollisionConfiguration();

    HYP_LOG_INFO("Physics", "Initialized Bullet physics driver!");
  }

  //--------------------------------------------------------------
  void BulletPhysicsDriver::FixedUpdate(float32 delta_time) { }

  //--------------------------------------------------------------
  void BulletPhysicsDriver::Shutdown() {
    delete m_collision_configuration;
  }

}
