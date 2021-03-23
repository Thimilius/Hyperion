//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/modules/mono/managed/mono_managed_method.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/modules/mono/mono_scripting_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    //--------------------------------------------------------------
    MonoManagedMethod::MonoManagedMethod(MonoMethod *mono_method) {
        m_mono_method = mono_method;
    }

    //--------------------------------------------------------------
    MonoObject *MonoManagedMethod::Invoke(void *object, void **parameters) {
        HYP_ASSERT(m_mono_method);

        MonoObject *exception = nullptr;
        MonoObject *result = mono_runtime_invoke(m_mono_method, object, parameters, &exception);
        if (exception != nullptr) {
            MonoScriptingDriver::PrintUnhandledException(exception);
        }

        return result;
    }

}