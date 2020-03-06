#pragma once

#include <type_traits>
#include <typeindex>

#include "hyperion/entity/entity_message.hpp"
#include "hyperion/entity/components/transform.hpp"

namespace Hyperion {

    class World;

    enum class EntityPrimitive {
        Quad,
        Plane,
        Cube,
        Sphere,

        DirectionalLight,
        PointLight,
        SpotLight,

        Camera
    };

    using EntityTag = String;

    // NOTE: Should we allow multiple components of the same type?
    class Entity : public Object, public EntityMessageListener {
        HYP_OBJECT(Entity, Object);
    private:
        Transform m_transform;
        Map<ObjectType, Component *> m_components;
        
        World *m_world = nullptr;
        Set<EntityTag> m_tags;

        Vector<EntityMessageListener *> m_message_listeners;
    public:
        inline World *GetWorld() const { return m_world; }
        inline Transform *GetTransform() { return &m_transform; }

        void OnMessage(EntityMessage message) override;

        template<typename T, typename =
            std::enable_if_t<std::is_base_of<Component, T>::value &&
            !std::is_same<Component, T>::value &&
            !std::is_base_of<Transform, T>::value &&
            std::is_default_constructible<T>::value>>
        T *AddComponent() {
            ObjectType type = T::GetTypeStatic();
            HYP_ASSERT_MESSAGE(m_components.find(type) == m_components.end(), "Failed to add component because a component with the same type already exists!");

            T *component = new T();

            Component *entity_component = component;
            entity_component->m_entity = this;
            m_components[type] = entity_component;

            entity_component->OnCreate();

            return component;
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value>>
        T *GetComponent() const {
            ObjectType type = T::GetTypeStatic();
            auto iterator = m_components.find(type);
            if (iterator != m_components.end()) {
                return static_cast<T *>(iterator->second);
            } else {
                return nullptr;
            }
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value>>
        T *GetComponentInChildren() const {
            ObjectType type = T::GetTypeStatic();
            T *component = nullptr;

            auto iterator = m_components.find(type);
            if (iterator != m_components.end()) {
                component = static_cast<T *>(iterator->second);
            } else {
                for (Transform *child : m_transform.m_children) {
                    component = child->GetEntity()->GetComponentInChildren<T>();
                    if (component) break;
                }
            }

            return component;
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value>>
        T *GetComponentInParent() const {
            ObjectType type = T::GetTypeStatic();
            T *component = nullptr;
            
            auto iterator = m_components.find(type);
            if (iterator != m_components.end()) {
                component = static_cast<T *>(iterator->second);
            } else {
                Transform *parent = m_transform.m_parent;
                while (parent != nullptr) {
                    auto &parent_components = parent->GetEntity()->m_components;
                    iterator = parent_components.find(type);
                    if (iterator != parent_components.end()) {
                        component = static_cast<T *>(iterator->second);
                        break;
                    }

                    parent = parent->m_parent;
                }
            }
            
            return component;
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value>>
        Vector<T *> GetComponentsInChildren() const {
            ObjectType type = T::GetTypeStatic();
            Vector<T *> components;

            auto iterator = m_components.find(type);
            if (iterator != m_components.end()) {
                components.push_back(static_cast<T *>(iterator->second););
            }
            for (Transform *child : m_transform.m_children) {
                Vector<T *> child_components = child->GetEntity()->GetComponentsInChildren<T>();
                components.insert(components.end(), child_components.begin(), child_components.end());
            }

            return components;
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value>>
        Vector<T *> GetComponentsInParent() const {
            ObjectType type = T::GetTypeStatic();
            Vector<T *> components;

            auto iterator = m_components.find(type);
            if (iterator != m_components.end()) {
                components.push_back(static_cast<T *>(iterator->second););
            }
            Transform *parent = m_transform.m_parent;
            while (parent != nullptr) {
                auto &parent_components = parent->GetEntity()->m_components;
                iterator = parent_components.find(type);
                if (iterator != parent_components.end()) {
                    components.push_back(static_cast<T *>(iterator->second););
                }

                parent = parent->m_parent;
            }

            return components;
        }

        inline const Set<EntityTag> &GetTags() const { return m_tags; }
        inline bool HasTag(const EntityTag &tag) const { return m_tags.find(tag) != m_tags.end(); }
        inline bool AddTag(const EntityTag &tag) { return m_tags.insert(tag).second; }
        inline void RemoveTag(const EntityTag &tag) { m_tags.erase(tag); }

        void RegisterMessageListener(EntityMessageListener *listener);
        void UnregisterMessageListener(EntityMessageListener *listener);

        static Entity *Create(const String &name = "New Entity", Vec3 position = Vec3::Zero(), Quaternion rotation = Quaternion::Identity(), Transform *parent = nullptr, World *world = nullptr);
        static Entity *CreatePrimitive(EntityPrimitive primitive);
    protected:
        void OnDestroy() override;
    private:
        Entity(const String &name) : Object(name) { }
        ~Entity() = default;

        void OnCreate(Vec3 position, Quaternion rotation, Transform *parent, World *world);

        static String GetPrimitiveName(EntityPrimitive primitive);

        friend class Object;
        friend class Component;
    };

}