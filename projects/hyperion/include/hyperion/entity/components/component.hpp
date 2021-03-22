#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/core/object/object.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Entity;
    class RectTransform;
    class Transform;
    class World;
    class WorldManager;
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    class Component : public Object {
        HYP_REFLECT();
    public:
        inline Entity *GetEntity() const { return m_entity; }

        Transform *GetTransform() const;
        World *GetWorld() const;

        virtual void Serialize(ISerializationStream &stream) override;
        virtual void Deserialize(IDeserializationStream &stream, ReferenceContext &context) override;
    protected:
        Component() = default;
        Component(const String &name) : Object(name) { }
    protected:
        virtual void OnCreate() { }
        virtual void OnDestroy() override;
    private:
        Entity *m_entity = nullptr;
    private:
        friend class Hyperion::Entity;
        friend class Hyperion::RectTransform;
        friend class Hyperion::WorldManager;
    };

}