#pragma once

namespace Hyperion {

    enum class EntityMessageType {
        TransformChanged,
        ComponentDestroyed
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