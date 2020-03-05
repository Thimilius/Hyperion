#pragma once

#include "hyperion/entity/components/behaviour.hpp"

namespace Hyperion {

    enum class ColliderType {
        BoxCollider,
        SphereCollider
    };

    class Collider : public Behaviour {
        HYP_OBJECT(Collider, Behaviour);
    public:
        virtual ColliderType GetColliderType() const = 0;
    protected:
        Collider() = default;
        Collider(const String & name) : Behaviour(name) {}
        virtual ~Collider() = default;
    };

}