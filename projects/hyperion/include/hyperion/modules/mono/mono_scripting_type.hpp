#pragma once

#include "hyperion/scripting/scripting_type.hpp"

#include <mono/jit/jit.h>

namespace Hyperion::Scripting {

    class MonoScriptingType : public ScriptingType {
    public:
        MonoScriptingType(MonoClass *managed_class);
    public:
        bool IsDerivedFrom(ScriptingType *scripting_type) const override;
    private:
        MonoClass *m_managed_class;
    };

}