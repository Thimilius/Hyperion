#include "hyppch.hpp"

#include "hyperion/entity/components/physics/box_collider.hpp"

#include "hyperion/entity/world.hpp"
#include "hyperion/physics/physics_world.hpp"

namespace Hyperion {

    void BoxCollider::OnMessage(EntityMessage message) {
        Collider::OnMessage(message);

        if (message.type == EntityMessageType::TransformChanged) {
            if (IsActiveAndEnabled()) {
                GetWorld()->GetPhysicsWorld()->UpdateBoxColliderTransform(this);
            }
        }
    }

    void BoxCollider::OnCreate() {
        Collider::OnCreate();

        GetWorld()->GetPhysicsWorld()->AddBoxCollider(this);
    }

    void BoxCollider::NotifyColliderChange() {
        if (IsActiveAndEnabled()) {
            GetWorld()->GetPhysicsWorld()->UpdateBoxCollider(this);
        }
    }

    BoxCollider *BoxCollider::Create() {
        return new BoxCollider();
    }

}