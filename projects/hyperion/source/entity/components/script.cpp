#include "hyppch.hpp"

#include "hyperion/entity/components/script.hpp"

#include "hyperion/entity/entity.hpp"

namespace Hyperion {

    Script *Script::Create() {
        return new Script();
    }

    void Script::OnDestroy() {
        // We explicitly do not call the base method here.
        GetEntity()->DispatchMessage({ EntityMessageType::ScriptDestroyed, this });
    }

}