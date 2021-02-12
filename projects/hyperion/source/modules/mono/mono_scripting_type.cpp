//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/mono/mono_scripting_type.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    //--------------------------------------------------------------
    MonoScriptingType::MonoScriptingType(MonoClass *managed_class) {
        m_managed_class = managed_class;
    }

    //--------------------------------------------------------------
    bool MonoScriptingType::IsDerivedFrom(ScriptingType *scripting_type) const {
        MonoScriptingType *mono_scripting_type = static_cast<MonoScriptingType *>(scripting_type);
        return mono_class_is_subclass_of(m_managed_class, mono_scripting_type->m_managed_class, true);
    }

}