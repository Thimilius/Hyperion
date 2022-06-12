//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/bullet/bullet_physics_driver.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/modules/bullet/bullet_physics_world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Physics {

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

  //--------------------------------------------------------------
  IPhysicsWorld *BulletPhysicsDriver::CreateWorld(World *world) {
    return new BulletPhysicsWorld(world, this);
  }

  //--------------------------------------------------------------
  IPhysicsWorld *BulletPhysicsDriver::CopyWorld(World *world, IPhysicsWorld *physics_world) {
    return new BulletPhysicsWorld(world, this, physics_world);
  }

  //--------------------------------------------------------------
  void BulletPhysicsDriver::DestroyWorld(IPhysicsWorld *physics_world) {
    delete physics_world;
  }

}
