#pragma once

//---------------------- Library Includes ----------------------
#include <mono/jit/jit.h>

//---------------------- Project Includes ----------------------
#include "hyperion/modules/mono/managed/mono_managed_method.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    class MonoManagedAssembly {
    public:
        MonoManagedAssembly() = default;
        MonoManagedAssembly(MonoAssembly *mono_assembly);
    public:
        MonoManagedMethod FindMethod(const String &description);
        MonoClass *FindClass(const String &name_space, const String &name);
    private:
        MonoAssembly *m_mono_assembly = nullptr;
        MonoImage *m_mono_assembly_image = nullptr;
    };

}