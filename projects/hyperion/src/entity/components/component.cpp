#include "hyppch.hpp"

#include "hyperion/entity/components/component.hpp"

#include "hyperion/entity/entity.hpp"

namespace Hyperion {

    Transform *Component::GetTransform() const {
        return m_entity->GetTransform();
    }

    void Component::OnDestroy() {
        GetEntity()->OnMessage({ EntityMessageType::ComponentDestroyed, this });
    }

}