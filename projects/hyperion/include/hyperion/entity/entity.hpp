#pragma once

#include <type_traits>
#include <typeindex>

#include "hyperion/entity/entity_message.hpp"
#include "hyperion/entity/components/transform_component.hpp"

namespace Hyperion {

    class World;

    enum class EntityPrimitive {
        Sphere,
        Cube,
        Plane
    };

    using EntityTag = String;

    // NOTE: Should we allow multiple components of the same type?
    class Entity : public Object, public EntityMessageListener {
        HYP_OBJECT(Entity, Object);
    private:
        TransformComponent m_transform;
        Map<ObjectType, EntityComponent *> m_components;
        
        World *m_world = nullptr;
        Set<EntityTag> m_tags;

        Vector<EntityMessageListener *> m_message_listeners;
    public:
        inline World *GetWorld() const { return m_world; }
        inline TransformComponent *GetTransform() { return &m_transform; }

        void OnMessage(EntityMessage message) override;

        template<typename T>
        T *AddComponent() {
            static_assert(std::is_base_of<EntityComponent, T>::value, "Template parameter must derive from EntityComponent");
            static_assert(!std::is_same<EntityComponent, T>::value, "Template parameter must derive from EntityComponent");
            static_assert(!std::is_base_of<TransformComponent, T>::value, "Can not add an additional TransformComponent");

            ObjectType type = T::GetStaticType();
            HYP_ASSERT_MESSAGE(m_components.find(type) == m_components.end(), "Failed to add component because a component with the same type already exists!");

            T *component = new T();

            EntityComponent *entity_component = component;
            entity_component->m_entity = this;
            m_components[type] = entity_component;

            entity_component->OnCreate();

            return component;
        }

        template<typename T>
        T *GetComponent() const {
            static_assert(std::is_base_of<EntityComponent, T>::value, "Template parameter must derive from EntityComponent");

            ObjectType type = T::GetStaticType();
            auto iterator = m_components.find(type);
            if (iterator != m_components.end()) {
                return (T*)iterator->second;
            } else {
                return nullptr;
            }
        }

        template<typename T>
        T *GetComponentInChildren() const {
            static_assert(std::is_base_of<EntityComponent, T>::value, "Template parameter must derive from EntityComponent");

            ObjectType type = T::GetStaticType();
            T *component = nullptr;

            auto iterator = m_components.find(type);
            if (iterator != m_components.end()) {
                component = (T *)iterator->second;
            } else {
                for (TransformComponent *child : m_transform.m_children) {
                    component = child->GetEntity()->GetComponentInChildren<T>();
                    if (component) break;
                }
            }

            return component;
        }

        template<typename T>
        T *GetComponentInParent() const {
            static_assert(std::is_base_of<EntityComponent, T>::value, "Template parameter must derive from EntityComponent");

            ObjectType type = T::GetStaticType();
            T *component = nullptr;
            
            auto iterator = m_components.find(type);
            if (iterator != m_components.end()) {
                component = (T *)iterator->second;
            } else {
                TransformComponent *parent = m_transform.m_parent;
                while (parent != nullptr) {
                    auto &parent_components = parent->GetEntity()->m_components;
                    iterator = parent_components.find(type);
                    if (iterator != parent_components.end()) {
                        component = (T *)iterator->second;
                        break;
                    }

                    parent = parent->m_parent;
                }
            }
            
            return component;
        }

        template<typename T>
        Vector<T *> GetComponentsInChildren() const {
            static_assert(std::is_base_of<EntityComponent, T>::value, "Template parameter must derive from EntityComponent");

            ObjectType type = T::GetStaticType();
            Vector<T *> components;

            auto iterator = m_components.find(type);
            if (iterator != m_components.end()) {
                components.push_back((T *)iterator->second);
            }
            for (TransformComponent *child : m_transform.m_children) {
                Vector<T *> child_components = child->GetEntity()->GetComponentsInChildren<T>();
                components.insert(components.end(), child_components.begin(), child_components.end());
            }

            return components;
        }

        template<typename T>
        Vector<T *> GetComponentsInParent() const {
            static_assert(std::is_base_of<EntityComponent, T>::value, "Template parameter must derive from EntityComponent");

            ObjectType type = T::GetStaticType();
            Vector<T *> components;

            auto iterator = m_components.find(type);
            if (iterator != m_components.end()) {
                components.push_back((T *)iterator->second);
            }
            TransformComponent *parent = m_transform.m_parent;
            while (parent != nullptr) {
                auto &parent_components = parent->GetEntity()->m_components;
                iterator = parent_components.find(type);
                if (iterator != parent_components.end()) {
                    components.push_back((T *)iterator->second);
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

        static Entity *Create(const String &name = "New Entity", Vec3 position = Vec3::Zero(), Quaternion rotation = Quaternion::Identity(), TransformComponent *parent = nullptr);
        static Entity *CreatePrimitive(EntityPrimitive primitive);
    protected:
        void OnDestroy() override;
    private:
        Entity(const String &name) : Object(name) { }
        ~Entity() = default;

        void OnCreate(Vec3 position, Quaternion rotation, TransformComponent *parent);

        friend class Object;
        friend class EntityComponent;
    };

}