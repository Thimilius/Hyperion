#pragma once

//---------------------- Library Includes ----------------------
#include <mono/jit/jit.h>

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    class MonoManagedString {
    public:
        MonoManagedString(const char *string);
        MonoManagedString(const String &string);
        MonoManagedString(MonoString *mono_string);
    public:
        inline MonoString *GetMonoString() const { return m_mono_string; }
        inline String GetString() const { return m_string; }
    private:
        MonoString *m_mono_string;
        String m_string;
    };

}