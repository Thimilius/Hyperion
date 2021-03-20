//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/mono/managed/managed_domain.hpp"

//---------------------- Library Includes ----------------------
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-gc.h>
#include <mono/metadata/threads.h>

//---------------------- Project Includes ----------------------
#include "hyperion/modules/mono/mono_scripting_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    //--------------------------------------------------------------
    ManagedDomain::ManagedDomain(MonoDomain *domain) {
        HYP_ASSERT(domain);
        m_domain = domain;
    }

    //--------------------------------------------------------------
    void ManagedDomain::SetActive() {
        mono_domain_set(m_domain, true);
    }

    //--------------------------------------------------------------
    void ManagedDomain::Finalize() {
        if (!mono_domain_finalize(m_domain, 2000)) {
            HYP_LOG_ERROR("Scripting", "Domain finalization timeout!");
        }
        mono_gc_collect(mono_gc_max_generation());
    }

    //--------------------------------------------------------------
    void ManagedDomain::Unload() {
        if (m_domain == nullptr) {
            return;
        }

        Finalize();

        MonoObject *exception = nullptr;
        mono_domain_try_unload(m_domain, &exception);
        if (exception != nullptr) {
            MonoScriptingDriver::PrintUnhandledException(exception);
        }
    }

    //--------------------------------------------------------------
    ManagedAssembly ManagedDomain::LoadAssembly(const String &path) {
        MonoAssembly *assembly = mono_domain_assembly_open(m_domain, path.c_str());
        HYP_ASSERT(assembly);
        return ManagedAssembly(assembly);
    }

    //--------------------------------------------------------------
    ManagedDomain ManagedDomain::Create(char *name) {
        MonoDomain *domain = mono_domain_create_appdomain(name, nullptr);
        return ManagedDomain(domain);
    }

    //--------------------------------------------------------------
    void ManagedDomain::SetCurrentMainThread() {
        mono_thread_set_main(mono_thread_current());
    }

}