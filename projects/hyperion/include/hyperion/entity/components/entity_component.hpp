#pragma once

#include "hyperion/entity/object.hpp"

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

        template<typename T>
        T *GetComponent() const { return m_entity->GetComponent<T>(); }
        template<typename T>
        T *GetComponentInChildren() const { return m_entity->GetComponentInChildren<T>(); }
        template<typename T>
        T *GetComponentInParent() const { return m_entity->GetComponentInParent<T>(); }
        template<typename T>
        Vector<T *> GetComponentsInChildren() const { return m_entity->GetComponentsInChildren<T>(); }
        template<typename T>
        Vector<T *> GetComponentsInParent() const { return m_entity->GetComponentsInParent<T>(); }
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