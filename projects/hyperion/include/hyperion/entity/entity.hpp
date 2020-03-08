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
    // Currently the GetComponents implementations assume only one component of a certain type
    class Entity : public Object {
        HYP_OBJECT(Entity, Object);
    private:
        Transform m_transform;
        Map<ObjectType, Component *> m_components;
        
        World *m_world = nullptr;
        Set<EntityTag> m_tags;
        bool m_active = true;

        Vector<EntityMessageListener *> m_message_listeners;
    public:
        inline World *GetWorld() const { return m_world; }
        inline Transform *GetTransform() { return &m_transform; }

        inline bool IsActive() const { return m_active; }
        inline void SetActive(bool active) {
            m_active = active;
            NotifyActivationChanged();
        }
        bool IsActiveInHierarchy() const;

        void DispatchMessage(EntityMessage message);

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

            for (auto pair : m_components) {
                if (pair.second->IsBase(type)) {
                    return static_cast<T *>(pair.second);
                }
            }

            return nullptr;
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value>>
        T *GetComponentInChildren() const {
            ObjectType type = T::GetTypeStatic();

            T *component = GetComponent<T>();
            if (component) {
                return component;
            } else {
                for (Transform *child : m_transform.m_children) {
                    component = child->GetEntity()->GetComponentInChildren<T>();
                    if (component) {
                        return component;
                    }
                }
            }

            return nullptr;
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value>>
        T *GetComponentInParent() const {
            ObjectType type = T::GetTypeStatic();
            
            Component *component = GetComponent<T>();
            if (component) {
                return component;
            } else {
                Transform *parent = m_transform.m_parent;
                if (parent != nullptr) {
                    return parent->GetEntity()->GetComponentInParent<T>();
                }
            }

            return nullptr;
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value>>
        Vector<T *> GetComponentsInChildren() const {
            ObjectType type = T::GetTypeStatic();
            Vector<T *> components;

            for (auto pair : m_components) {
                if (pair.second->IsBase(type)) {
                    components.push_back(static_cast<T *>(pair.second));
                }
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

            for (auto pair : m_components) {
                if (pair.second->IsBase(type)) {
                    components.push_back(static_cast<T *>(pair.second));
                }
            }
            Transform *parent = m_transform.m_parent;
            if (parent != nullptr) {
                Vector<T *> parent_components = parent->GetEntity()->GetComponentsInParent<T>();
                components.insert(components.end(), parent_components.begin(), parent_components.end());
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

        void NotifyActivationChanged();

        void OnCreate(Vec3 position, Quaternion rotation, Transform *parent, World *world);

        static String GetPrimitiveName(EntityPrimitive primitive);

        friend class Object;
        friend class Component;
    };

}