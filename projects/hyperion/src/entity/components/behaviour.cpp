#include "hyppch.hpp"

#include "hyperion/entity/components/behaviour.hpp"

#include "hyperion/entity/entity.hpp"

namespace Hyperion {

    void Behaviour::NotifyBehaviourEnabledChanged() {
        GetEntity()->DispatchMessage({ EntityMessageType::BehaviourEnabledChanged, (void *)m_enabled });
    }

}