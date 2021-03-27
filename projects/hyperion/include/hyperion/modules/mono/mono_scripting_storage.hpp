#pragma once

//---------------------- Library Includes ----------------------
#include <mono/jit/jit.h>

//---------------------- Project Includes ----------------------
#include "hyperion/modules/mono/mono_scripting_instance.hpp"
#include "hyperion/scripting/scripting_object.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion::Scripting {
    class MonoScriptingDriver;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    class MonoScriptingStorage final {
    public:
        static MonoScriptingType *GetOrCreateScriptingType(MonoClass *mono_class);
    private:
        MonoScriptingStorage() = delete;
        ~MonoScriptingStorage() = delete;
    private:
        static void Clear();
    private:
        inline static Map<MonoClass *, std::unique_ptr<MonoScriptingType>> s_scripting_types;
    private:
        friend class Hyperion::Scripting::MonoScriptingDriver;
    };

}