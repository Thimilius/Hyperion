#pragma once

//---------------------- Library Includes ----------------------
#include <mono/jit/jit.h>

//---------------------- Project Includes ----------------------
#include "hyperion/modules/mono/managed/mono_managed_exception.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    class MonoManagedMethod {
    public:
        MonoManagedMethod() = default;
        MonoManagedMethod(MonoMethod *mono_method);
    public:
        MonoObject *Invoke(void *object, void **parameters);
        MonoObject *Invoke(void *object, void **parameters, MonoManagedException &exception);
    private:
        MonoMethod *m_mono_method = nullptr;
    };

}