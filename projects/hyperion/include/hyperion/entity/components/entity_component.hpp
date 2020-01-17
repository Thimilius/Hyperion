#pragma once

#include "hyperion/core/object.hpp"

namespace Hyperion {

    class Entity;

    class EntityComponent : public Object {
        HYP_OBJECT(EntityComponent, Object);
    private:
        Entity *m_entity;
    public:
        inline Entity *GetEntity() const { return m_entity; }
    protected:
        EntityComponent() = default;
        virtual ~EntityComponent() = default;

        virtual void OnCreate() { }
        virtual void OnDestroy() { }
    private:
        friend class Entity;
    };

}