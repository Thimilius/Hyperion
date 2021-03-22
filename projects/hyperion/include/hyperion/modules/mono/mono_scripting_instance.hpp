#pragma once

//---------------------- Library Includes ----------------------
#include <mono/jit/jit.h>

//---------------------- Project Includes ----------------------
#include "hyperion/scripting/scripting_instance.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    class MonoScriptingType : public ScriptingType {
    public:
        MonoScriptingType(MonoClass *mono_class);
    public:
        MonoClass *GetMonoClass() const { return m_mono_class; }
    private:
        MonoClass *m_mono_class;
    };

    class MonoScriptingInstance : public ScriptingInstance {
    public:
        MonoScriptingInstance(MonoObject *mono_object, bool is_script_component);
    public:
        ScriptingType *GetScriptingType() const override;

        void SendMessage(ScriptingMessage message) override;

        inline MonoObject *GetMonoObject() const { return m_mono_object; }
    private:
        void CallMethod(const char *name, int32 parameter_count, void **args);
    private:
        MonoObject *m_mono_object;
        bool m_is_script_component;
    };

}