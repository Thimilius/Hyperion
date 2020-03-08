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

    void SphereCollider::OnDestroy() {
        GetWorld()->GetPhysicsWorld()->RemoveCollider(this);

        Collider::OnDestroy();
    }

    void SphereCollider::NotifyColliderChange() {
        if (IsActiveAndEnabled()) {
            GetWorld()->GetPhysicsWorld()->UpdateSphereCollider(this);
        }
    }

}