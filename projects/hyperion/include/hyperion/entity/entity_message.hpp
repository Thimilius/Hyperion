#pragma once

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Component;
    class Script;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class EntityMessage {
        TransformChanged,
        TransformParentChanged,

        ActivationChanged,
    };

    class IEntityMessageListener {
    public:
        virtual ~IEntityMessageListener() = default;
    public:
        virtual void OnEntityMessage(EntityMessage message) = 0;
    };

}