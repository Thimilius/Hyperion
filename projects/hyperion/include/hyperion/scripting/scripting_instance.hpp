#pragma once

//-------------------- Definition Namespace --------------------
namespace Hyperion::Scripting {

    enum class ScriptingMessage {
        OnCreate,
        OnUpdate,
        OnDestroy,
    };

    class ScriptingType {
    public:
        virtual ~ScriptingType() = default;
    };

    class ScriptingInstance {
    public:
        virtual ~ScriptingInstance() = default;
    public:
        virtual ScriptingType *GetScriptingType() const = 0;

        virtual void SendMessage(ScriptingMessage message) = 0;
    };

}