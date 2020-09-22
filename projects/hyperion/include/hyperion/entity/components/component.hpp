#pragma once

#include "hyperion/entity/object.hpp"

namespace Hyperion {
    class Entity;
    class ObjectManager;
    class Transform;
    class World;
}

namespace Hyperion {

    class Component : public Object {
        HYP_OBJECT(Component, Object);
    public:
        inline Entity *GetEntity() const { return m_entity; }

        Transform *GetTransform() const;
        World *GetWorld() const;
    protected:
        Component() = default;
        Component(const String &name) : Object(name) { }
        virtual ~Component() = default;

        void RegisterUpdate();

        virtual void OnCreate() { }
        virtual void OnUpdate(f32 delta_time) { }
        virtual void OnDestroy() override;
    private:
        Entity *m_entity;
    private:
        friend class Hyperion::Entity;
        friend class Hyperion::ObjectManager;
    };

}