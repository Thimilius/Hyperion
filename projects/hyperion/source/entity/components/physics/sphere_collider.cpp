//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/physics/sphere_collider.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/world.hpp"
#include "hyperion/physics/physics_world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void SphereCollider::OnMessage(EntityMessage message) {
        Collider::OnMessage(message);

        if (message.type == EntityMessageType::TransformChanged) {
            if (IsActiveAndEnabled()) {
                GetWorld()->GetPhysicsWorld()->UpdateSphereColliderTransform(this);
            }
        }
    }

    //--------------------------------------------------------------
    void SphereCollider::OnCreate() {
        Collider::OnCreate();

        GetWorld()->GetPhysicsWorld()->AddSphereCollider(this);
    }

    //--------------------------------------------------------------
    void SphereCollider::NotifyColliderChange() {
        if (IsActiveAndEnabled()) {
            GetWorld()->GetPhysicsWorld()->UpdateSphereCollider(this);
        }
    }

    //--------------------------------------------------------------
    SphereCollider *SphereCollider::Create() {
        return new SphereCollider();
    }

}
