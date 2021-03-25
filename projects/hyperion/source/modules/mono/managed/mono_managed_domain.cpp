//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/mono/managed/mono_managed_domain.hpp"

//---------------------- Library Includes ----------------------
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-gc.h>
#include <mono/metadata/threads.h>

//---------------------- Project Includes ----------------------
#include "hyperion/modules/mono/mono_scripting_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    //--------------------------------------------------------------
    MonoManagedDomain::MonoManagedDomain(MonoDomain *mono_domain) {
        HYP_ASSERT(mono_domain);

        m_mono_domain = mono_domain;
    }

    //--------------------------------------------------------------
    void MonoManagedDomain::SetActive() {
        mono_domain_set(m_mono_domain, true);
    }

    //--------------------------------------------------------------
    void MonoManagedDomain::Finalize() {
        if (!mono_domain_finalize(m_mono_domain, 2000)) {
            HYP_LOG_ERROR("Scripting", "Domain finalization timeout!");
        }
        mono_gc_collect(mono_gc_max_generation());
    }

    //--------------------------------------------------------------
    void MonoManagedDomain::Unload() {
        if (m_mono_domain == nullptr) {
            return;
        }

        Finalize();

        MonoObject *mono_exception = nullptr;
        mono_domain_try_unload(m_mono_domain, &mono_exception);
        if (mono_exception != nullptr) {
            MonoManagedException(mono_exception).Print();
        }
    }

    //--------------------------------------------------------------
    MonoManagedAssembly MonoManagedDomain::LoadAssembly(const String &path) {
        MonoAssembly *assembly = mono_domain_assembly_open(m_mono_domain, path.c_str());
        HYP_ASSERT(assembly);
        return MonoManagedAssembly(assembly);
    }

    //--------------------------------------------------------------
    MonoManagedDomain MonoManagedDomain::Create(char *name) {
        MonoDomain *domain = mono_domain_create_appdomain(name, nullptr);
        return MonoManagedDomain(domain);
    }

    //--------------------------------------------------------------
    void MonoManagedDomain::SetCurrentMainThread() {
        mono_thread_set_main(mono_thread_current());
    }

}