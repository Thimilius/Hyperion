#pragma once

//---------------------- Library Includes ----------------------
#include <mono/jit/jit.h>

//---------------------- Project Includes ----------------------
#include "hyperion/scripting/scripting_instance.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    class MonoScriptingInstance : public ScriptingInstance {
    public:
        MonoScriptingInstance(MonoObject *managed_object, bool is_script_component);
    public:
        inline MonoObject *GetManagedObject() const { return m_managed_object; }

        void SendMessage(ScriptingMessage message) override;
    private:
        void CallMethod(const char *name, int32 parameter_count, void **args);
    private:
        MonoObject *m_managed_object;
        bool m_is_script_component;
    };

}