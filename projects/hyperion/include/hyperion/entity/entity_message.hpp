#pragma once

namespace Hyperion {

    enum class EntityMessageType {
        ComponentDestroyed,

        TransformChanged,
        ActivationChanged
    };

    struct EntityMessage {
        EntityMessageType type;
        void *parameter;
    };

    class IEntityMessageListener {
    public:
        virtual ~IEntityMessageListener() = default;
    public:
        virtual void OnMessage(EntityMessage message) = 0;
    };

}