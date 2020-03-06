#pragma once

#include "hyperion/entity/entity_message.hpp"
#include "hyperion/entity/components/behaviour.hpp"

namespace Hyperion {

    enum class ColliderType {
        BoxCollider,
        SphereCollider
    };

    class Collider : public Behaviour, public EntityMessageListener {
        HYP_OBJECT(Collider, Behaviour);
    public:
        virtual ColliderType GetColliderType() const = 0;

        void OnMessage(EntityMessage message) override;
    protected:
        Collider() = default;
        Collider(const String & name) : Behaviour(name) {}
        virtual ~Collider() = default;

        void OnCreate() override;
        void OnDestroy() override;
    };

}