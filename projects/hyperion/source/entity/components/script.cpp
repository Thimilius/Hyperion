//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/components/script.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/entity.hpp"

//------------------------- Namespaces -------------------------
using namespace Hyperion::Scripting;

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void Script::SendMessage(ScriptingMessage message) {
        HYP_ASSERT(GetScriptingInstance());
        GetScriptingInstance()->SendMessage(message);
    }

    //--------------------------------------------------------------
    Script *Script::Create() {
        return new Script();
    }

}