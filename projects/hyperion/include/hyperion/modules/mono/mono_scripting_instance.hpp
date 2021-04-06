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
        MonoScriptingInstance(MonoObject *mono_object);
        virtual ~MonoScriptingInstance() override;
    public:
        ScriptingType *GetScriptingType() const override;

        void SendMessage(ScriptingMessage message) override { }

        inline MonoObject *GetMonoObject() const { return m_mono_object; }
    private:
        MonoObject *m_mono_object;
        MonoGCHandle m_gc_handle;
    };

    class MonoScriptingInstanceScript : public MonoScriptingInstance {
    public:
        MonoScriptingInstanceScript(MonoObject *mono_object);
    public:
        void SendMessage(ScriptingMessage message) override;
    };

}