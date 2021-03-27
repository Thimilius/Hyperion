//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/mono/mono_scripting_storage.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    //--------------------------------------------------------------
    MonoScriptingType *MonoScriptingStorage::GetOrCreateScriptingType(MonoClass *mono_class) {
        auto it = s_scripting_types.find(mono_class);
        if (it != s_scripting_types.end()) {
            return it->second.get();
        } else {
            MonoScriptingType *mono_scripting_type = new MonoScriptingType(mono_class);
            s_scripting_types[mono_class].reset(mono_scripting_type);
            return mono_scripting_type;
        }
    }

    //--------------------------------------------------------------
    void MonoScriptingStorage::Clear() {
        s_scripting_types.clear();
    }

}