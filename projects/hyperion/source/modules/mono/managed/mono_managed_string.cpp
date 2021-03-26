//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/mono/managed/mono_managed_string.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/modules/mono/mono_scripting_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    //--------------------------------------------------------------
    MonoManagedString::MonoManagedString(const char *string) {
        HYP_ASSERT(string);

        m_mono_string = mono_string_new(MonoScriptingDriver::GetRuntimeDomain()->GetMonoDomain(), string);
        m_string = string;
    }

    //--------------------------------------------------------------
    MonoManagedString::MonoManagedString(const String &string) : MonoManagedString(string.c_str()) { }

    //--------------------------------------------------------------
    MonoManagedString::MonoManagedString(MonoString *mono_string) {
        HYP_ASSERT(mono_string);

        m_mono_string = mono_string;

        char *buffer = mono_string_to_utf8(mono_string);
        m_string = buffer;
        mono_free(buffer);
    }

}