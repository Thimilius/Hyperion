#pragma once

namespace Hyperion::Scripting {

    class ScriptingType {
    public:
        virtual ~ScriptingType() = default;
    public:
        virtual bool IsDerivedFrom(ScriptingType *scripting_type) const = 0;
    };

}