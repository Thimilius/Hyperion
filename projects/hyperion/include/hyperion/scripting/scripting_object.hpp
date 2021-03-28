#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/object/type.hpp"
#include "hyperion/scripting/scripting_instance.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    class ScriptingObject {
        HYP_REFLECT();
    public:
        ScriptingObject() = default;
        virtual ~ScriptingObject();
    public:
        inline ScriptingInstance *GetScriptingInstance() const { return m_scripting_instance; }
        inline void SetScriptingInstance(ScriptingInstance *scripting_instance) { m_scripting_instance = scripting_instance; }
        void ClearScriptingInstance();
    private:
        ScriptingInstance *m_scripting_instance = nullptr;
    };

}