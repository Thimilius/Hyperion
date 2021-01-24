#include "hyppch.hpp"

#include "hyperion/entity/components/physics/sphere_collider.hpp"

#include "hyperion/entity/world.hpp"
#include "hyperion/physics/physics_world.hpp"

namespace Hyperion {

    void SphereCollider::OnMessage(EntityMessage message) {
        Collider::OnMessage(message);

        if (message.type == EntityMessageType::TransformChanged) {
            if (IsActiveAndEnabled()) {
                GetWorld()->GetPhysicsWorld()->UpdateSphereColliderTransform(this);
            }
        }
    }

    void SphereCollider::OnCreate() {
        Collider::OnCreate();

        GetWorld()->GetPhysicsWorld()->AddSphereCollider(this);
    }

    void SphereCollider::NotifyColliderChange() {
        if (IsActiveAndEnabled()) {
            GetWorld()->GetPhysicsWorld()->UpdateSphereCollider(this);
        }
    }

    SphereCollider *SphereCollider::Create() {
        return new SphereCollider();
    }

}

HYP_REFLECT_REGISTER_BEGIN
{
    Registration<SphereCollider>("SphereCollider")
        .constructor(select_overload<SphereCollider *()>(&SphereCollider::Create))(DefaultConstructorPolicy);
}
HYP_REFLECT_REGISTER_END
