#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/modules/mono/mono_scripting_driver.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    class MonoScriptingHelper final {
    public:
        template<typename T> static T *GetNativeObjectAs(MonoObject *managed_object) { return reinterpret_cast<T *>(MonoScriptingDriver::GetNativeObject(managed_object)); }
    private:
        MonoScriptingHelper() = delete;
        ~MonoScriptingHelper() = delete;
    };

}