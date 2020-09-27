#pragma once

#include "hyperion/core/object.hpp"

namespace Hyperion {
    class Entity;
    class Transform;
    class UITransform;
    class World;
    class WorldManager;
}

namespace Hyperion {

    class Component : public Object {
        RTTR_ENABLE(Object);
    public:
        inline Entity *GetEntity() const { return m_entity; }

        Transform *GetTransform() const;
        World *GetWorld() const;
    protected:
        Component() = default;
        Component(const String &name) : Object(name) { }
        virtual ~Component() = default;

        void RegisterForUpdate();
        void UnregisterForUpdate();

        virtual void OnCreate() { }
        virtual void OnUpdate(f32 delta_time) { }
        virtual void OnDestroy() override;
    private:
        Entity *m_entity;
    private:
        friend class Hyperion::Entity;
        friend class Hyperion::UITransform;
        friend class Hyperion::WorldManager;
    };

}