#pragma once

#include "hyperion/entity/object.hpp"

namespace Hyperion {

    class Entity;
    class Transform;

    class Component : public Object {
        HYP_OBJECT(Component, Object);
    private:
        Entity *m_entity;
    public:
        inline Entity *GetEntity() const { return m_entity; }

        Transform *GetTransform() const;
    protected:
        Component() { }
        Component(const String &name) : Object(name) { }
        virtual ~Component() = default;

        void RegisterUpdate();

        virtual void OnCreate() { }
        virtual void OnUpdate(f32 delta_time) { }
        virtual void OnDestroy() override;
    private:
        friend class Entity;
        friend class ObjectManager;
    };

}