//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/physics/box_collider.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/world.hpp"
#include "hyperion/physics/physics_world.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void BoxCollider::OnEntityMessage(EntityMessage message) {
        Collider::OnEntityMessage(message);

        if (message == EntityMessage::TransformChanged) {
            if (IsActiveAndEnabled()) {
                GetWorld()->GetPhysicsWorld()->UpdateBoxColliderTransform(this);
            }
        }
    }

    //--------------------------------------------------------------
    void BoxCollider::Serialize(ISerializationStream &stream) {
        Collider::Serialize(stream);

        stream.WriteVec3("origin", m_origin);
        stream.WriteVec3("size", m_size);
    }

    //--------------------------------------------------------------
    void BoxCollider::Deserialize(IDeserializationStream &stream) {
        Collider::Deserialize(stream);

        m_origin = stream.ReadVec3("origin");
        m_size = stream.ReadVec3("size");
    }

    //--------------------------------------------------------------
    void BoxCollider::OnCreate() {
        Collider::OnCreate();

        GetWorld()->GetPhysicsWorld()->AddBoxCollider(this);
    }

    //--------------------------------------------------------------
    void BoxCollider::NotifyColliderChange() {
        if (IsActiveAndEnabled()) {
            GetWorld()->GetPhysicsWorld()->UpdateBoxCollider(this);
        }
    }

    //--------------------------------------------------------------
    HYP_REFLECT_BEGIN(BoxCollider)
    HYP_REFLECT_BASE(Collider)
    HYP_REFLECT_CONSTRUCTOR([]() { return new BoxCollider(); })
    HYP_REFLECT_END()

}
