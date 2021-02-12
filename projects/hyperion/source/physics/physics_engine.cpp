//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/physics/physics_engine.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/world_manager.hpp"
#ifdef HYP_PHYSICS_BULLET
#include "hyperion/modules/bullet/bullet_physics_driver.hpp"
#endif

//-------------------- Definition Namespace --------------------
namespace Hyperion::Physics {

    //--------------------------------------------------------------
    bool PhysicsEngine::Raycast(Ray ray, RaycastResult &result, float32 distance) {
        return WorldManager::GetActiveWorld()->GetPhysicsWorld()->Raycast(ray, result, distance);
    }

    //--------------------------------------------------------------
    void PhysicsEngine::Initialize() {
#ifdef HYP_PHYSICS_BULLET
        s_physics_driver = new BulletPhysicsDriver();
#else
        s_physics_driver = new DummyPhysicsDriver();
#endif

        s_physics_driver->Initialize();
    }

    //--------------------------------------------------------------
    void PhysicsEngine::FixedUpdate(float32 delta_time) {
        s_physics_driver->FixedUpdate(delta_time);
    }

    //--------------------------------------------------------------
    void PhysicsEngine::Shutdown() {
        s_physics_driver->Shutdown();
        delete s_physics_driver;
    }

}