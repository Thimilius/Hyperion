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
    void SphereCollider::OnEntityMessage(EntityMessage message) {
        Collider::OnEntityMessage(message);

        if (message.type == EntityMessageType::TransformChanged) {
            if (IsActiveAndEnabled()) {
                GetWorld()->GetPhysicsWorld()->UpdateSphereColliderTransform(this);
            }
        }
    }

    //--------------------------------------------------------------
    void SphereCollider::Serialize(ISerializationStream &stream) {
        Collider::Serialize(stream);

        stream.WriteFloat("radius", m_radius);
    }

    //--------------------------------------------------------------
    void SphereCollider::Deserialize(IDeserializationStream &stream, ReferenceContext &context) {
        Collider::Deserialize(stream, context);

        m_radius = stream.ReadFloat("radius");
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
    HYP_REFLECT_BEGIN(SphereCollider)
    HYP_REFLECT_BASE(Collider)
    HYP_REFLECT_CONSTRUCTOR([]() { return new SphereCollider(); })
    HYP_REFLECT_END()

}
