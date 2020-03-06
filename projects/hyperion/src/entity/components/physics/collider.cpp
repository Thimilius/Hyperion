#include "hyppch.hpp"

#include "hyperion/entity/components/physics/collider.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/world.hpp"
#include "hyperion/physics/physics_world.hpp"

namespace Hyperion {

    void Collider::OnMessage(EntityMessage message) {
        switch (message.type) {
            case EntityMessageType::TransformChanged: {
                if (IsEnabled()) {
                    GetWorld()->GetPhysicsWorld()->UpdateColliderTransform(this);
                }
                break;
            }
            case EntityMessageType::BehaviourEnabledChanged: {
                GetWorld()->GetPhysicsWorld()->UpdateColliderActivation(this);
                break;
            }
            default: break;
        }
    }

    void Collider::OnCreate() {
        Behaviour::OnCreate();

        GetEntity()->RegisterMessageListener(this);
    }

    void Collider::OnDestroy() {
        GetEntity()->UnregisterMessageListener(this);

        Behaviour::OnDestroy();
    }

}