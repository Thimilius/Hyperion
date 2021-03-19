#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/scripting/scripting_instance.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    class ScriptingObject {
    public:
        ScriptingObject() = default;
        virtual ~ScriptingObject() = default;
    public:
        inline ScriptingInstance *GetScriptingInstance() const { return m_scripting_instance; }
        inline void SetScriptingInstance(ScriptingInstance *scripting_instance) { m_scripting_instance = scripting_instance; }
    private:
        ScriptingInstance *m_scripting_instance = nullptr;
    };

}