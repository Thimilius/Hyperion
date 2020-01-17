#include "hyppch.hpp"

#include "hyperion/entity/components/entity_component.hpp"

#include "hyperion/entity/entity.hpp"

namespace Hyperion {

    void EntityComponent::OnDestroy() {
        GetEntity()->OnEvent({ EntityEventType::ComponentDestroyed, this });
    }

}