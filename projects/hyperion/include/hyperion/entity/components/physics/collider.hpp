#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/math/bounding_box.hpp"
#include "hyperion/entity/entity_message.hpp"
#include "hyperion/entity/components/behaviour.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    enum class ColliderType {
        BoxCollider,
        SphereCollider,
        MeshCollider
    };

    class Collider : public Behaviour, public IEntityMessageListener {
        HYP_REFLECT();
    public:
        void SetEnabled(bool enabled) override;

        virtual ColliderType GetColliderType() const = 0;
        
        virtual BoundingBox GetBoundingBox();

        virtual void OnEntityMessage(EntityMessage message) override;

        virtual void Serialize(ISerializationStream &stream) override;
        virtual void Deserialize(IDeserializationStream &stream) override;
    protected:
        Collider() = default;
        Collider(const String & name) : Behaviour(name) {}
        virtual ~Collider() override = default;
    protected:
        void OnCreate() override;
        void OnDestroy() override;
    };

}