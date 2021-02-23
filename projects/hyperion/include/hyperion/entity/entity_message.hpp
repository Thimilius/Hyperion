#pragma once

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Component;
    class Script;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class EntityMessageType {
        TransformChanged,
        TransformParentChanged,
        ActivationChanged,

        ComponentDestroyed,
        ScriptDestroyed,
    };

    struct EntityMessage {
        EntityMessageType type;

        union EntityMessageData {
            struct ComponentDestroyed {
                Component *component;
            } component_destroyed;
            
            struct ScriptDestroyed {
                Script *script;
            } script_destroyed;
        } data;
    };

    class IEntityMessageListener {
    public:
        virtual ~IEntityMessageListener() = default;
    public:
        virtual void OnMessage(EntityMessage message) = 0;
    };

}