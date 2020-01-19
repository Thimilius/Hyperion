#include "hyppch.hpp"

#include "hyperion/entity/components/entity_component.hpp"

#include "hyperion/entity/entity.hpp"

namespace Hyperion {

    TransformComponent *EntityComponent::GetTransform() const {
        return m_entity->GetTransform();
    }

    void EntityComponent::OnDestroy() {
        GetEntity()->OnEvent({ EntityEventType::ComponentDestroyed, this });
    }

}