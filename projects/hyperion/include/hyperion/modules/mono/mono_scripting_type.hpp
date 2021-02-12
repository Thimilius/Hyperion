#pragma once

//---------------------- Library Includes ----------------------
#include <mono/jit/jit.h>

//---------------------- Project Includes ----------------------
#include "hyperion/scripting/scripting_type.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    class MonoScriptingType : public ScriptingType {
    public:
        MonoScriptingType(MonoClass *managed_class);
    public:
        inline MonoClass *GetManagedClass() const { return m_managed_class; }

        bool IsDerivedFrom(ScriptingType *scripting_type) const override;
    private:
        MonoClass *m_managed_class;
    };

}