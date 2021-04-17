#pragma once

//---------------------- Project Includes ----------------------
#include "hyperion/common.hpp"
#include "hyperion/core/color.hpp"
#include "hyperion/entity/entity.hpp"
#include "hyperion/physics/physics_world.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Component;
    class Entity;
    class Light;
    class MeshRenderer;
    class Transform;
    class WorldManager;

    namespace Rendering {
        class RenderEngine;
    }
}

//-------------------- Definition Namespace --------------------
namespace Hyperion {
    
    struct WorldEnvironment {
        struct AmbientLight {
            float32 intensity = 0.2f;
            Color color = Color::White();
        } ambient_light;
    };

    class World final : public Scripting::ScriptingObject, public ISerializable {
        HYP_REFLECT();
    public:
        inline String GetName() const { return m_name; }
        inline void SetName(const String &name) { m_name = name; }

        inline WorldEnvironment &GetEnvironment() { return m_environment; }
        inline Physics::PhysicsWorld *GetPhysicsWorld() const { return m_physics_world; }

        inline const Vector<Entity *> &GetRootEntites() const { return m_root_entities; }
        inline const Vector<Light *> &GetLights() const { return m_lights; }
        inline const Vector<MeshRenderer *> &GetMeshRenderers() const { return m_mesh_renderers; }

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

        void Serialize(ISerializationStream &stream) override;
        void Deserialize(IDeserializationStream &stream) override;
    private:
        World() = default;
        World(const String &name);
        ~World();
    private:
        void OnUpdate(float32 delta_time);
        void OnLateUpdate(float32 delta_time);

        void OnAfterDeserialization();
        void SetWorldReferenceRecursive(Entity *entity);

        void AddRootEntity(Entity *entity);
        void RemoveRootEntity(Entity *entity);

        void RegisterComponentForUpdate(Component *component);
        void UnregisterComponentForUpdate(Component *component);

        void AddLight(Light *light);
        void RemoveLight(Light *light);
        void AddMeshRenderer(MeshRenderer *mesh_renderer);
        void RemoveMeshRenderer(MeshRenderer *mesh_renderer);
    private:
        String m_name;
        WorldEnvironment m_environment;
        Physics::PhysicsWorld *m_physics_world;

        Vector<Entity *> m_root_entities;
        Vector<Component *> m_components_to_update;

        Vector<Light *> m_lights;
        Vector<MeshRenderer *> m_mesh_renderers;
    private:
        friend class Hyperion::Component;
        friend class Hyperion::Entity;
        friend class Hyperion::Light;
        friend class Hyperion::MeshRenderer;
        friend class Hyperion::Transform;
        friend class Hyperion::WorldManager;
        friend class Hyperion::Rendering::RenderEngine;
    };

}