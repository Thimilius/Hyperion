#pragma once

namespace Hyperion {
    class Object;
}

namespace Hyperion::Scripting {

    class ScriptingInstance {
    public:
        virtual ~ScriptingInstance() = default;
    public:
        inline Object *GetObject() const { return m_object; }

        virtual void OnDestroy() = 0;
    private:
        Object *m_object;
    private:
        friend class Hyperion::Object;
    };

}