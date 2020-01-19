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
    protected:
        EntityComponent() = default;
        virtual ~EntityComponent() = default;

        virtual void OnCreate() { }
        virtual void OnDestroy() override;
    private:
        friend class Entity;
    };

}