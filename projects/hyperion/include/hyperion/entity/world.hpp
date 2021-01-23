#pragma once

#include <queue>

#include "hyperion/common.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/entity/entity.hpp"
#include "hyperion/physics/physics_world.hpp"

namespace Hyperion {
    class Entity;
    class Transform;
    class WorldManager;
}

namespace Hyperion {
    
    struct WorldEnvironment {
        struct AmbientLight {
            float32 intensity = 0.2f;
            Color color = Color::White();
        } ambient_light;
    };

    class World final : public Object {
        HYP_REFLECT(Object);
    public:
        inline WorldEnvironment &GetEnvironment() { return m_environment; }

        inline Physics::PhysicsWorld *GetPhysicsWorld() const { return m_physics_world; }

        inline const Vector<Entity *> &GetRootEntites() const { return m_root_entities; }

        template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value>>
        T *FindComponentOfType() {
            // This is a depth first search
            for (Entity *entity : m_root_entities) {
                T *component = entity->GetComponentInChildren<T>();
                if (component) {
                    return component;
                }
            }

            return nullptr;
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value>>
        Vector<T *> FindComponentsOfType() {
            Vector<T *> components;

            // This is a depth first search
            for (Entity *entity : m_root_entities) {
                Vector<T *> child_components = entity->GetComponentsInChildren<T>();
                components.insert(components.end(), child_components.begin(), child_components.end());
            }

            return components;
        }
    private:
        World() : World("World") {}
        World(const String &name);
        ~World() override;

        void AddRootEntity(Entity *entity);
        void RemoveRootEntity(Entity *entity);
    private:
        WorldEnvironment m_environment;

        Physics::PhysicsWorld *m_physics_world;

        Vector<Entity *> m_root_entities;
    private:
        friend class Hyperion::Entity;
        friend class Hyperion::Transform;
        friend class Hyperion::WorldManager;
    };

}