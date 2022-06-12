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

  class BulletPhysicsDriver final : public IPhysicsDriver {
  public:
    PhysicsBackend GetBackend() const override { return PhysicsBackend::Bullet; }

    void Initialize() override;
    void FixedUpdate(float32 delta_time) override;
    void Shutdown() override;

    IPhysicsWorld *CreateWorld(World *world) override;
    IPhysicsWorld *CopyWorld(World *world, IPhysicsWorld *physics_world) override;
    void DestroyWorld(IPhysicsWorld *physics_world) override;

    inline btDefaultCollisionConfiguration *GetCollisionConfiguration() const { return m_collision_configuration; }
  private:
    btDefaultCollisionConfiguration *m_collision_configuration = nullptr;
  };

}
