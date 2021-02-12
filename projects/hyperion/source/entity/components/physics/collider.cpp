//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/physics/collider.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/world.hpp"
#include "hyperion/physics/physics_world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void Collider::SetEnabled(bool enabled) {
        Behaviour::SetEnabled(enabled);

        GetWorld()->GetPhysicsWorld()->UpdateColliderActivation(this);
    }

    //--------------------------------------------------------------
    BoundingBox Collider::GetBounds() {
        return GetWorld()->GetPhysicsWorld()->GetBounds(this);
    }

    //--------------------------------------------------------------
    void Collider::OnMessage(EntityMessage message) {
        if (message.type == EntityMessageType::ActivationChanged) {
            GetWorld()->GetPhysicsWorld()->UpdateColliderActivation(this);
        }
    }

    //--------------------------------------------------------------
    void Collider::OnCreate() {
        Behaviour::OnCreate();

        GetEntity()->RegisterMessageListener(this);
    }

    //--------------------------------------------------------------
    void Collider::OnDestroy() {
        GetWorld()->GetPhysicsWorld()->RemoveCollider(this);
        GetEntity()->UnregisterMessageListener(this);

        Behaviour::OnDestroy();
    }

}
