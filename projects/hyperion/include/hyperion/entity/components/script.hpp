#pragma once

#include "hyperion/entity/components/component.hpp"
#include "hyperion/scripting/scripting_type.hpp"

namespace Hyperion {
    class Entity;
}

namespace Hyperion {

    class Script final : public Component {
        HYP_REFLECT(Component);
    public:
        inline Scripting::ScriptingType *GetScriptingType() const { return m_scripting_type; }
        
        void SendMessage(Scripting::ScriptingMessage message);
    public:
        static Script *Create();
    private:
        Script() = default;
    private:
        Scripting::ScriptingType *m_scripting_type;
    private:
        friend class Hyperion::Entity;
    };

}