//------------------------ Header Guard ------------------------
#pragma once

//---------------------- Library Includes ----------------------
#include <bullet/btBulletCollisionCommon.h>

//---------------------- Project Includes ----------------------
#include "hyperion/physics/physics_driver.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Physics {
  class BulletPhysicsWorld;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Physics {

  class BulletPhysicsDriver : public IPhysicsDriver {
  public:
    PhysicsBackend GetBackend() const override { return PhysicsBackend::Bullet; }

    IPhysicsWorld *CreatePhysicsWorld(World *world) override;
    void DestroyPhysicsWorld(IPhysicsWorld *world) override;

    void Initialize() override;
    void FixedUpdate(float32 delta_time) override;
    void Shutdown() override;
  private:
    btDefaultCollisionConfiguration *m_collision_configuration;
  private:
    friend class Hyperion::Physics::BulletPhysicsWorld;
  };

}
