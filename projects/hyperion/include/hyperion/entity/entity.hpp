#pragma once

#include <type_traits>
#include <typeindex>

#include "hyperion/core/layer.hpp"
#include "hyperion/entity/entity_message.hpp"
#include "hyperion/entity/components/transform.hpp"
#include "hyperion/entity/components/ui/ui_transform.hpp"

namespace Hyperion {
    class Component;
    class Object;
    class World;
    class UITransform;
}

namespace Hyperion {

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
    // Currently the GetComponent implementations assume only one component of a certain type
    class Entity : public Object {
        HYP_OBJECT(Entity, Object);
    public:
        inline World *GetWorld() const { return m_world; }

        inline bool IsActive() const { return m_active; }
        inline void SetActive(bool active) {
            if (active != m_active) {
                m_active = active;
                NotifyActivationChanged();
            }
        }
        bool IsActiveInHierarchy() const;

        inline LayerMask GetLayer() const { return m_layer; }
        inline void SetLayer(LayerMask layer) { m_layer = layer; }

        void DispatchMessage(EntityMessage message);

        template<typename T, typename =
            std::enable_if_t<std::is_base_of<Component, T>::value &&
            !std::is_same<Component, T>::value &&
            !std::is_same<Transform, T>::value &&
            std::is_default_constructible<T>::value>>
        T *AddComponent() {
            ObjectType type = T::GetStaticType();
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
            ObjectType type = T::GetStaticType();

            for (auto [component_type, component] : m_components) {
                if (component->IsBase(type)) {
                    return static_cast<T *>(component);
                }
            }

            return nullptr;
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value>>
        T *GetComponentInChildren() const {
            ObjectType type = T::GetStaticType();

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
            ObjectType type = T::GetStaticType();
            
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
            ObjectType type = T::GetStaticType();
            Vector<T *> components;

            for (auto [component_type, component] : m_components) {
                if (component->IsBase(type)) {
                    components.push_back(static_cast<T *>(component));
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
            ObjectType type = T::GetStaticType();
            Vector<T *> components;

            for (auto [component_type, component] : m_components) {
                if (component->IsBase(type)) {
                    components.push_back(static_cast<T *>(component));
                }
            }
            Transform *parent = m_transform.m_parent;
            if (parent != nullptr) {
                Vector<T *> parent_components = parent->GetEntity()->GetComponentsInParent<T>();
                components.insert(components.end(), parent_components.begin(), parent_components.end());
            } 
            
            return components;
        }

        inline Transform *GetTransform() { return m_transform; }

        inline const Set<EntityTag> &GetTags() const { return m_tags; }
        inline bool HasTag(const EntityTag &tag) const { return m_tags.find(tag) != m_tags.end(); }
        inline bool AddTag(const EntityTag &tag) { return m_tags.insert(tag).second; }
        inline void RemoveTag(const EntityTag &tag) { m_tags.erase(tag); }

        void RegisterMessageListener(IEntityMessageListener *listener);
        void UnregisterMessageListener(IEntityMessageListener *listener);

        static Entity *Create(const String &name = "New Entity", const Vec3 &position = Vec3::Zero(), const Quaternion &rotation = Quaternion::Identity(), Transform *parent = nullptr, World *world = nullptr);
        static Entity *CreatePrimitive(EntityPrimitive primitive, const Vec3 &position = Vec3::Zero(), const Quaternion &rotation = Quaternion::Identity(), Transform *parent = nullptr, World *world = nullptr);
    protected:
        void OnDestroy() override;
    private:
        Entity(const String &name) : Object(name) { }
        ~Entity() = default;

        void NotifyActivationChanged();

        void OnCreate(const Vec3 &position, const Quaternion &rotation, Transform *parent, World *world);

        static String GetPrimitiveName(EntityPrimitive primitive);
    private:
        World *m_world = nullptr;
        bool m_active = true;

        LayerMask m_layer = LayerMask::Default;

        Map<ObjectType, Component *> m_components;
        Transform *m_transform = nullptr;
        Set<EntityTag> m_tags;

        Vector<IEntityMessageListener *> m_message_listeners;
    private:
        friend class Hyperion::Component;
        friend class Hyperion::Object;
        friend class Hyperion::UITransform;
    };

}