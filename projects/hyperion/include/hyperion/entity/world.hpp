#pragma once

#include <queue>

#include "hyperion/common.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/entity/entity.hpp"
#include "hyperion/physics/physics_world.hpp"

namespace Hyperion {
    class Camera;
    class Entity;
    class Light;
    class MeshRenderer;
    class Transform;
    class WorldManager;

    namespace Rendering {
        class TextureCubemap;
    }

    namespace UI {
        class Canvas;
    }
}

namespace Hyperion {
    
    struct WorldEnvironment {
        Rendering::TextureCubemap *skybox;

        struct AmbientLight {
            f32 intensity = 0.2f;
            Color color = Color::White();
        } ambient_light;
    };

    class World final : public Object {
        RTTR_ENABLE(Object);
    public:
        inline WorldEnvironment &GetEnvironment() { return m_environment; }

        inline Physics::PhysicsWorld *GetPhysicsWorld() const { return m_physics_world; }

        inline const Vector<Entity *> &GetRootEntites() const { return m_root_entities; }

        inline const Vector<Camera *> &GetCameras() const { return m_cameras; }
        inline const Vector<MeshRenderer *> &GetMeshRenderers() const { return m_mesh_renderers; }
        inline const Vector<Light *> &GetLights() const { return m_lights; }
        inline const Vector<UI::Canvas *> &GetCanvases() const { return m_canvases; }

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
        ~World();

        void AddRootEntity(Entity *entity);
        void RemoveRootEntity(Entity *entity);

        void AddCamera(Camera *camera);
        void RemoveCamera(Camera *camera);
        void AddMeshRenderer(MeshRenderer *mesh_renderer);
        void RemoveMeshRenderer(MeshRenderer *mesh_renderer);
        void AddLight(Light *light);
        void RemoveLight(Light *light);
        void AddCanvas(UI::Canvas *ui_canvas);
        void RemoveCanvas(UI::Canvas *ui_canvas);
    private:
        WorldEnvironment m_environment;

        Physics::PhysicsWorld *m_physics_world;

        Vector<Entity *> m_root_entities;

        Vector<Camera *> m_cameras;
        Vector<MeshRenderer *> m_mesh_renderers;
        Vector<Light *> m_lights;
        Vector<UI::Canvas *> m_canvases;
    private:
        friend class Hyperion::Camera;
        friend class Hyperion::Entity;
        friend class Hyperion::Light;
        friend class Hyperion::MeshRenderer;
        friend class Hyperion::Transform;
        friend class Hyperion::WorldManager;
        friend class Hyperion::UI::Canvas;
    };

}