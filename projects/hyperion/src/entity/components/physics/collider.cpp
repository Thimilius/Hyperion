#include "hyppch.hpp"

#include "hyperion/entity/components/physics/collider.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/world.hpp"
#include "hyperion/physics/physics_world.hpp"

namespace Hyperion {

    void Collider::SetEnabled(bool enabled) {
        GetWorld()->GetPhysicsWorld()->UpdateColliderActivation(this);
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