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

}