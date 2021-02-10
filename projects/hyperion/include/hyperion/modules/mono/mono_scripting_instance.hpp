#pragma once

#include "hyperion/scripting/scripting_instance.hpp"

#include <mono/jit/jit.h>

namespace Hyperion::Scripting {

    class MonoScriptingInstance : public ScriptingInstance {
    public:
        MonoScriptingInstance(MonoObject *mono_object);
    public:
        inline MonoObject *GetMonoObject() const { return m_mono_object; }

        void OnDestroy() override;
    private:
        MonoObject *m_mono_object;
    };

}