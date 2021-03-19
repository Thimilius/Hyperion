//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/mono/mono_scripting_helper.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    //--------------------------------------------------------------
    MonoString *MonoScriptingHelper::NewString(const String &string) {
        return mono_string_new(MonoScriptingDriver::GetRuntimeDomain(), string.c_str());
    }

}