#pragma once

#include "hyperion/scripting/scripting_instance.hpp"

#include <mono/jit/jit.h>

namespace Hyperion::Scripting {

    class MonoScriptingInstance : public ScriptingInstance {
    public:
        MonoScriptingInstance(MonoObject *managed_object);
    public:
        inline MonoObject *GetManagedObject() const { return m_managed_object; }

        void OnDestroy() override;
    private:
        MonoObject *m_managed_object;
    };

}