#pragma once

#include <queue>

#include "hyperion/common.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/entity/entity.hpp"
#include "hyperion/physics/physics_world.hpp"

namespace Hyperion {
    class Entity;
    class MeshRenderer;
    class Transform;
    class WorldManager;

    namespace Rendering {
        class RenderEngine;
    }
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

            for (Entity *entity : m_root_entities) {
                Vector<T *> child_components = entity->GetComponentsInChildren<T>();
                components.insert(components.end(), child_components.begin(), child_components.end());
            }

            return components;
        }
    protected:
        void OnDestroy() override;
    private:
        World() = default;
        World(const String &name);
    private:
        void AddRootEntity(Entity *entity);
        void RemoveRootEntity(Entity *entity);

        inline const Vector<MeshRenderer *> &GetMeshRenderers() const { return m_mesh_renderers; }
        void AddMeshRenderer(MeshRenderer *mesh_renderer);
        void RemoveMeshRenderer(MeshRenderer *mesh_renderer);
    private:
        static World *Create();
    private:
        WorldEnvironment m_environment;
        Physics::PhysicsWorld *m_physics_world;

        Vector<Entity *> m_root_entities;

        Vector<MeshRenderer *> m_mesh_renderers;
    private:
        friend class Hyperion::Entity;
        friend class Hyperion::MeshRenderer;
        friend class Hyperion::Transform;
        friend class Hyperion::WorldManager;
        friend class Hyperion::Rendering::RenderEngine;
    };

}