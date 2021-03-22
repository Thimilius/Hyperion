#pragma once

//---------------------- Library Includes ----------------------
#include <mono/jit/jit.h>

//---------------------- Project Includes ----------------------
#include "hyperion/modules/mono/managed/mono_managed_assembly.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    class MonoManagedDomain {
    public:
        MonoManagedDomain() = default;
        MonoManagedDomain(MonoDomain *mono_domain);
    public:
        inline MonoDomain *GetMonoDomain() const { return m_mono_domain; }

        void SetActive();

        MonoManagedAssembly LoadAssembly(const String &path);

        void Finalize();
        void Unload();
    public:
        static MonoManagedDomain Create(char *name);
        static void SetCurrentMainThread();
    private:
        MonoDomain *m_mono_domain = nullptr;
    };

}