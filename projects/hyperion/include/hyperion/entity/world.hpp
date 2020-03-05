#pragma once

#include <queue>

#include "hyperion/common.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/entity/entity.hpp"
#include "hyperion/physics/physics_world.hpp"

namespace Hyperion {

    namespace Rendering {
        class TextureCubemap;
    }
    class MeshRenderer;
    class Light;
    class Camera;

    struct WorldEnvironment {
        struct Background {
            enum class Mode {
                Color,
                Skybox
            };

            Mode mode = Mode::Color;
            Color color = Color::Black();
            Ref<Rendering::TextureCubemap> skybox;
        } background;

        struct AmbientLight {
            f32 intensity = 0.2f;
            Color color = Color::White();
        } ambient_light;
    };

    class World {
    private:
        WorldEnvironment m_environment;

        Physics::PhysicsWorld *m_physics_world;

        Vector<Entity *> m_root_entities;

        Vector<Camera *> m_cameras;
        Vector<MeshRenderer *> m_mesh_renderers;
        Vector<Light *> m_lights;
    public:
        inline WorldEnvironment &GetEnvironment() { return m_environment; }

        inline Physics::PhysicsWorld *GetPhysicsWorld() const { return m_physics_world; }

        inline const Vector<Entity *> &GetRootEntites() const { return m_root_entities; }

        inline const Vector<Camera *> &GetCameras() const { return m_cameras; }
        inline const Vector<MeshRenderer *> &GetMeshRenderers() const { return m_mesh_renderers; }
        inline const Vector<Light *> &GetLights() const { return m_lights; }

        template<typename T, typename = std::enable_if<std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value>::type>
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

        template<typename T, typename = std::enable_if<std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value>::type>
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
        World();
        ~World();

        void AddRootEntity(Entity *entity);
        void RemoveRootEntity(Entity *entity);

        void AddCamera(Camera *camera);
        void RemoveCamera(Camera *camera);
        void AddMeshRenderer(MeshRenderer *mesh_renderer);
        void RemoveMeshRenderer(MeshRenderer *mesh_renderer);
        void AddLight(Light *light);
        void RemoveLight(Light *light);

        friend class Entity;
        friend class WorldManager;
        friend class Transform;
        friend class MeshRenderer;
        friend class Light;
        friend class Camera;
    };

}