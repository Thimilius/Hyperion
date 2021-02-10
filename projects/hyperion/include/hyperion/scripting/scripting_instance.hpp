#pragma once

namespace Hyperion {
    class Object;
}

namespace Hyperion::Scripting {

    class ScriptingInstance {
    public:
        virtual ~ScriptingInstance() = default;

        virtual void OnDestroy() = 0;
    private:
        Object *m_object;
    private:
        friend class Hyperion::Object;
    };

}