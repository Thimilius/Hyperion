#pragma once

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
        virtual void SendMessage(ScriptingMessage message) = 0;
    };

}