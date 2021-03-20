#pragma once

//---------------------- Library Includes ----------------------
#include <mono/jit/jit.h>

//---------------------- Project Includes ----------------------
#include "hyperion/modules/mono/managed/managed_assembly.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    class ManagedDomain {
    public:
        ManagedDomain() = default;
        ManagedDomain(MonoDomain *domain);
    public:
        inline MonoDomain *GetMonoDomain() const { return m_domain; }

        void SetActive();

        ManagedAssembly LoadAssembly(const String &path);

        void Finalize();
        void Unload();
    public:
        static ManagedDomain Create(char *name);
        static void SetCurrentMainThread();
    private:
        MonoDomain *m_domain;
    };

}