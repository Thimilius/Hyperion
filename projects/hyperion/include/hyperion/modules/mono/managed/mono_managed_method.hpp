#pragma once

//---------------------- Library Includes ----------------------
#include <mono/jit/jit.h>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    class MonoManagedMethod {
    public:
        MonoManagedMethod() = default;
        MonoManagedMethod(MonoMethod *mono_method);
    public:
        MonoObject *Invoke(void *object, void **parameters);
    private:
        MonoMethod *m_mono_method = nullptr;
    };

}