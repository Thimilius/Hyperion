#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/entity/components/component.hpp"
#include "hyperion/scripting/scripting_type.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Entity;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Script final : public Component {
        HYP_REFLECT();
    public:
        inline Scripting::ScriptingType *GetScriptingType() const { return m_scripting_type; }
    private:
        Script() = default;
    private:
        Scripting::ScriptingType *m_scripting_type = nullptr;
    private:
        friend class Hyperion::Entity;
    };

}