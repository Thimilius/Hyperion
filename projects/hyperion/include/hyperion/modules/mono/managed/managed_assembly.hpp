#pragma once

//---------------------- Library Includes ----------------------
#include <mono/jit/jit.h>

//---------------------- Project Includes ----------------------
#include "hyperion/modules/mono/managed/managed_method.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    class ManagedAssembly {
    public:
        ManagedAssembly() = default;
        ManagedAssembly(MonoAssembly *assembly);
    public:
        ManagedMethod FindMethod(const String &description);
        MonoClass *FindClass(const String &name_space, const String &name);
    private:
        MonoAssembly *m_assembly;
        MonoImage *m_assembly_image;
    };

}