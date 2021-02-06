#pragma once

#include "hyperion/core/math/bounding_box.hpp"
#include "hyperion/entity/entity_message.hpp"
#include "hyperion/entity/components/behaviour.hpp"

namespace Hyperion {

    enum class ColliderType {
        BoxCollider,
        SphereCollider,
        MeshCollider
    };

    class Collider : public Behaviour, public IEntityMessageListener {
        HYP_REFLECT(Behaviour);
    public:
        void SetEnabled(bool enabled) override;

        virtual ColliderType GetColliderType() const = 0;
        
        virtual BoundingBox GetBounds();

        virtual void OnMessage(EntityMessage message);
    protected:
        Collider() = default;
        Collider(const String & name) : Behaviour(name) {}
        virtual ~Collider() override = default;
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    };

}