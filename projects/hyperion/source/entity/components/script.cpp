#include "hyppch.hpp"

#include "hyperion/entity/components/script.hpp"

#include "hyperion/entity/entity.hpp"

using namespace Hyperion::Scripting;

namespace Hyperion {

    void Script::SendMessage(ScriptingMessage message) {
        HYP_ASSERT(GetScriptingInstance());
        GetScriptingInstance()->SendMessage(message);
    }

    Script *Script::Create() {
        return new Script();
    }

    void Script::OnCreate() {
        Component::OnCreate();

        RegisterForUpdate();

        // We can not send the OnCreate scripting message here as the underlying scripting instance is not yet set.
    }

    void Script::OnUpdate(float32 delta_time) {
        SendMessage(ScriptingMessage::OnUpdate);
    }

    void Script::OnDestroy() {
        // The destroy scripting message gets send by Object and is therefore not necessary here.

        UnregisterForUpdate();

        GetEntity()->DispatchMessage({ EntityMessageType::ScriptDestroyed, this });

        // We explicitly do not call the base Component::OnDestroy method here.
    }

}