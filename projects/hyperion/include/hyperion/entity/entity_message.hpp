#pragma once

namespace Hyperion {

    enum class EntityMessageType {
        ComponentDestroyed,

        TransformChanged
    };

    struct EntityMessage {
        EntityMessageType type;
        void *parameter;
    };

    class EntityMessageListener {
    public:
        virtual ~EntityMessageListener() = default;

        virtual void OnMessage(EntityMessage message) = 0;
    };

}