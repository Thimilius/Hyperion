#pragma once

//---------------------- Library Includes ----------------------
#include <mono/metadata/object.h>

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    class MonoManagedException {
    public:
        MonoManagedException() = default;
        MonoManagedException(MonoObject *mono_exception);
        MonoManagedException(MonoException *mono_exception);
    public:
        bool IsValid() const { return m_mono_exception != nullptr; }

        inline MonoException *GetMonoException() const { return m_mono_exception; }

        void Print();
    private:
        MonoException *m_mono_exception = nullptr;
    };

}