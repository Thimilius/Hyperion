//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/mono/managed/managed_method.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/modules/mono/mono_scripting_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    //--------------------------------------------------------------
    ManagedMethod::ManagedMethod(MonoMethod *method) {
        m_method = method;
    }

    //--------------------------------------------------------------
    void ManagedMethod::Invoke(void *object, void **parameters) {
        HYP_ASSERT(m_method);

        MonoObject *exception = nullptr;
        mono_runtime_invoke(m_method, object, parameters, &exception);
        if (exception != nullptr) {
            MonoScriptingDriver::PrintUnhandledException(exception);
        }
    }

}