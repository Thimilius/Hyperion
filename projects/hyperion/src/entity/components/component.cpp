#include "hyppch.hpp"

#include "hyperion/entity/components/component.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/object_manager.hpp"

namespace Hyperion {

    Transform *Component::GetTransform() const {
        return m_entity->GetTransform();
    }

    World *Component::GetWorld() const {
        return m_entity->GetWorld();
    }

    void Component::RegisterUpdate() {
        ObjectManager::RegisterComponentForUpdate(this);
    }

    void Component::OnDestroy() {
        GetEntity()->OnMessage({ EntityMessageType::ComponentDestroyed, this });

        ObjectManager::UnregisterComponentForUpdate(this);
    }

}