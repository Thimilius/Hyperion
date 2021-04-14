#pragma once

//---------------------- Library Includes ----------------------
#include <mono/jit/jit.h>

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"
#include "hyperion/modules/mono/mono_scripting_storage.hpp"
#include "hyperion/modules/mono/mono_scripting_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {
    
    template<typename T>
    class MonoManagedArray {
    public:
        MonoManagedArray(const Vector<T *> &vector, MonoClass *element_class) {
            MonoDomain *mono_domain = MonoScriptingDriver::GetRuntimeDomain()->GetMonoDomain();
            m_mono_array = mono_array_new(mono_domain, element_class, vector.size());
            for (uint32 i = 0; i < vector.size(); i++) {
                mono_array_set(m_mono_array, MonoObject *, i, MonoScriptingStorage::GetOrCreateMonoObject(vector[i]));
            }
        }
    public:
        inline MonoArray *GetMonoArray() const { return m_mono_array; }
    private:
        MonoArray *m_mono_array;
    };

}