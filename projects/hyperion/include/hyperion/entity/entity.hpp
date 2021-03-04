#pragma once

//--------------- C++ Standard Library Includes ----------------
#include <type_traits>
#include <typeindex>

//---------------------- Project Includes ----------------------
#include "hyperion/entity/entity_message.hpp"
#include "hyperion/entity/layer.hpp"
#include "hyperion/entity/components/script.hpp"
#include "hyperion/entity/components/transform.hpp"

//-------------------- Forward Declarations --------------------
namespace Hyperion {
    class Component;
    class RectTransform;
    class World;
}

//-------------------- Definition Namespace --------------------
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
    // Currently the GetComponent implementations assume only one component of a certain type.
    class Entity final : public Object {
        HYP_REFLECT(Object);
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

        inline const Set<EntityTag> &GetTags() const { return m_tags; }
        inline bool HasTag(const EntityTag &tag) const { return m_tags.find(tag) != m_tags.end(); }
        inline bool AddTag(const EntityTag &tag) { return m_tags.insert(tag).second; }
        inline void RemoveTag(const EntityTag &tag) { m_tags.erase(tag); }

        const Map<ComponentType, Component *> &GetComponents() const { return m_components; }
        inline Transform *GetTransform() { return m_transform; }

        template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value && !std::is_same<Transform, T>::value>>
        T *AddComponent() {
            Type type = Type::get<T>();
            return static_cast<T *>(AddComponent(type));
        }
        Component *AddComponent(Type type);
        Script *AddScript(Scripting::ScriptingType *scripting_type);

        template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value>>
        T *GetComponent() const {
            Type type = Type::get<T>();
            Component *component = GetComponent(type);
            return static_cast<T *>(component);
        }
        Component *GetComponent(ComponentType type) const;

        template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value>>
        T *GetComponentInChildren() const {
            T *component = GetComponent<T>();
            if (component) {
                return component;
            } else {
                for (Transform *child : m_transform->m_children) {
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
            T *component = GetComponent<T>();
            if (component) {
                return component;
            } else {
                Transform *parent = m_transform->m_parent;
                if (parent != nullptr) {
                    return parent->GetEntity()->GetComponentInParent<T>();
                }
            }

            return nullptr;
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value>>
        Vector<T *> GetComponentsInChildren() const {
            Type type = Type::get<T>();

            Vector<T *> components;

            for (auto [component_type, component] : m_components) {
                if (component_type.IsDerivedFrom(type)) {
                    components.push_back(static_cast<T *>(component));
                }
            }
            for (Transform *child : m_transform->m_children) {
                Vector<T *> child_components = child->GetEntity()->GetComponentsInChildren<T>();
                components.insert(components.end(), child_components.begin(), child_components.end());
            }

            return components;
        }

        template<typename T, typename = std::enable_if_t<std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value>>
        Vector<T *> GetComponentsInParent() const {
            Type type = Type::get<T>();

            Vector<T *> components;

            for (auto [component_type, component] : m_components) {
                if (component_type.is_derived_from<T>()) {
                    components.push_back(static_cast<T *>(component));
                }
            }
            Transform *parent = m_transform->m_parent;
            if (parent != nullptr) {
                Vector<T *> parent_components = parent->GetEntity()->GetComponentsInParent<T>();
                components.insert(components.end(), parent_components.begin(), parent_components.end());
            } 
            
            return components;
        }

        void DispatchMessage(EntityMessage message);
        void RegisterMessageListener(IEntityMessageListener *listener);
        void UnregisterMessageListener(IEntityMessageListener *listener);

        virtual void Serialize(ISerializationStream &stream) override;
        virtual void Deserialize(IDeserializationStream &stream, ReferenceContext &context) override;
    public:
        static Entity *Create(const String &name, const Vec3 &position = Vec3::Zero(), const Quaternion &rotation = Quaternion::Identity(), Transform *parent = nullptr, World *world = nullptr);
        static Entity *CreateEmpty();
        static Entity *CreatePrimitive(EntityPrimitive primitive, const Vec3 &position = Vec3::Zero(), const Quaternion &rotation = Quaternion::Identity(), Transform *parent = nullptr, World *world = nullptr);
    protected:
        void OnDestroy() override;
    private:
        Entity() = default;
        Entity(const String &name) : Object(name) { }
    private:
        void NotifyActivationChanged();

        void MakeSureRequiredComponentsArePresent(Type type);
        void MakeSureRequiredComponentIsPresent(Type type, Metadata component_metadata);

        void OnCreate(const Vec3 &position, const Quaternion &rotation, Transform *parent, World *world);
    private:
        static Entity *Create();
        static String GetPrimitiveName(EntityPrimitive primitive);
    private:
        World *m_world = nullptr;
        bool m_active = true;

        LayerMask m_layer = LayerMask::Default;
        Set<EntityTag> m_tags;

        Map<ComponentType, Component *> m_components;
        Transform *m_transform = nullptr;

        Vector<IEntityMessageListener *> m_message_listeners;
    private:
        friend class Hyperion::Component;
        friend class Hyperion::RectTransform;
    };

}