#pragma once

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Object;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    enum class ScriptingMessage {
        OnCreate,
        OnUpdate,
        OnDestroy,
    };

    class ScriptingInstance {
    public:
        virtual ~ScriptingInstance() = default;
    public:
        inline Object *GetObject() const { return m_object; }

        virtual void SendMessage(ScriptingMessage message) = 0;
    private:
        Object *m_object;
    private:
        friend class Hyperion::Object;
    };

}