#pragma once

#include "hyperion/core/object.hpp"

namespace Hyperion {

    class Entity;
    class TransformComponent;

    class EntityComponent : public Object {
        HYP_OBJECT(EntityComponent, Object);
    private:
        Entity *m_entity;
    public:
        inline Entity *GetEntity() const { return m_entity; }
        TransformComponent *GetTransform() const;

        template<class T>
        T *GetComponent() const { return m_entity->GetComponent<T>(); }
    protected:
        EntityComponent() { }
        EntityComponent(const String &name) : Object(name) { }
        virtual ~EntityComponent() = default;

        virtual void OnCreate() { }
        virtual void OnDestroy() override;
    private:
        friend class Entity;
    };

}