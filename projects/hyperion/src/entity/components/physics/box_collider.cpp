#include "hyppch.hpp"

#include "hyperion/entity/components/physics/box_collider.hpp"

#include "hyperion/entity/world.hpp"
#include "hyperion/physics/physics_world.hpp"

namespace Hyperion {

    void BoxCollider::OnMessage(EntityMessage message) {
        Collider::OnMessage(message);

        if (message.type == EntityMessageType::TransformChanged) {
            if (IsEnabled()) {
                GetWorld()->GetPhysicsWorld()->UpdateBoxColliderTransform(this);
            }
        }
    }

    void BoxCollider::OnCreate() {
        Collider::OnCreate();

        GetWorld()->GetPhysicsWorld()->AddBoxCollider(this);
    }

    void BoxCollider::OnDestroy() {
        GetWorld()->GetPhysicsWorld()->RemoveCollider(this);

        Collider::OnDestroy();
    }

    void BoxCollider::NotifyColliderChange() {
        GetWorld()->GetPhysicsWorld()->UpdateBoxCollider(this);
    }

}