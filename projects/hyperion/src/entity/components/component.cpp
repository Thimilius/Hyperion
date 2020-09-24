#include "hyppch.hpp"

#include "hyperion/entity/components/component.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/world_manager.hpp"

namespace Hyperion {

    Transform *Component::GetTransform() const {
        return m_entity->GetTransform();
    }

    World *Component::GetWorld() const {
        return m_entity->GetWorld();
    }

    void Component::RegisterForUpdate() {
        WorldManager::RegisterComponentForUpdate(this);
    }

    void Component::UnregisterForUpdate() {
        WorldManager::UnregisterComponentForUpdate(this);
    }

    void Component::OnDestroy() {
        GetEntity()->DispatchMessage({ EntityMessageType::ComponentDestroyed, this });

        UnregisterForUpdate();
    }

}