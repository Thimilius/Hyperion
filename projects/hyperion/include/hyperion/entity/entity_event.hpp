#pragma once

namespace Hyperion {

    enum class EntityEventType {
        TransformChanged,
        ComponentDestroyed
    };

    struct EntityEvent {
        EntityEventType type;
        void *parameter;
    };

    class EntityEventListener {
    public:
        virtual ~EntityEventListener() = default;

        virtual void OnEvent(EntityEvent event) = 0;
    };

}