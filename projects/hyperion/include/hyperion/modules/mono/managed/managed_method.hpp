#pragma once

//---------------------- Library Includes ----------------------
#include <mono/jit/jit.h>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    class ManagedMethod {
    public:
        ManagedMethod() = default;
        ManagedMethod(MonoMethod *method);
    public:
        void Invoke(void *object, void **parameters);
    private:
        MonoMethod *m_method = nullptr;
    };

}