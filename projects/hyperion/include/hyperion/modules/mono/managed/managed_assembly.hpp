#pragma once

//---------------------- Library Includes ----------------------
#include <mono/jit/jit.h>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    class ManagedAssembly {
    public:
        ManagedAssembly() = default;
        ManagedAssembly(MonoAssembly *assembly);
    private:
        MonoAssembly *m_assembly;
        MonoImage *m_image;
    };

}