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
    BoundingBox Collider::GetBoundingBox() {
        return GetWorld()->GetPhysicsWorld()->GetBounds(this);
    }

    //--------------------------------------------------------------
    void Collider::OnEntityMessage(EntityMessage message) {
        if (message == EntityMessage::ActivationChanged) {
            GetWorld()->GetPhysicsWorld()->UpdateColliderActivation(this);
        }
    }

    //--------------------------------------------------------------
    void Collider::Serialize(ISerializationStream &stream) {
        Behaviour::Serialize(stream);
    }

    //--------------------------------------------------------------
    void Collider::Deserialize(IDeserializationStream &stream) {
        Behaviour::Deserialize(stream);
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

    //--------------------------------------------------------------
    HYP_REFLECT_BEGIN(Collider)
    HYP_REFLECT_BASE(Behaviour)
    HYP_REFLECT_END()

}
