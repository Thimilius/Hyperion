#pragma once

#include <type_traits>
#include <typeindex>

#include "hyperion/entity/entity_event.hpp"
#include "hyperion/entity/components/transform_component.hpp"

namespace Hyperion {

    class World;

    using EntityTag = String;

    class Entity : public Object, public EntityEventListener {
        HYP_OBJECT(Entity, Object);
    private:
        TransformComponent m_transform;
        Map<ObjectType, EntityComponent *> m_components;
        
        World *m_world = nullptr;
        Set<EntityTag> m_tags;

        Vector<EntityEventListener *> m_event_listeners;
    public:
        inline World *GetScene() const { return m_world; }
        inline TransformComponent *GetTransform() { return &m_transform; }

        void OnEvent(EntityEvent event) override;

        void OnCreate();
        void OnDestroy() override;

        template<class T>
        T *AddComponent() {
            // Template component constraint
            static_assert(std::is_base_of<EntityComponent, T>::value, "Template parameter must derive from EntityComponent");
            static_assert(!std::is_same<EntityComponent, T>::value, "Template parameter must derive from EntityComponent");
            static_assert(!std::is_base_of<TransformComponent, T>::value, "Can not add an additional TransformComponent");

            ObjectType type = T::GetStaticType();
            // NOTE: Should we allow multiple components of the same type?
            HYP_ASSERT_MESSAGE(m_components.find(type) == m_components.end(), "Failed to add component because a component with the same type already exists!");

            T *component = new T();

            EntityComponent *entity_component = component;
            entity_component->m_entity = this;
            m_components[type] = entity_component;

            entity_component->OnCreate();

            return component;
        }

        template<class T>
        T *GetComponent() const {
            // Template component constraint
            static_assert(std::is_base_of<EntityComponent, T>::value, "Template parameter must derive from EntityComponent");

            ObjectType type = T::GetStaticType();
            auto iterator = m_components.find(type);
            if (iterator != m_components.end()) {
                return (T*)iterator->second;
            } else {
                return nullptr;
            }
        }

        inline const Set<EntityTag> &GetTags() const { return m_tags; }
        inline bool HasTag(const EntityTag &tag) const { return m_tags.find(tag) != m_tags.end(); }
        inline bool AddTag(const EntityTag &tag) { return m_tags.insert(tag).second; }
        inline void RemoveTag(const EntityTag &tag) { m_tags.erase(tag); }

        void RegisterEventListener(EntityEventListener *listener);
        void UnregisterEventListener(EntityEventListener *listener);

        static Entity *Create(const String &name = "New Entity");
    private:
        Entity(const String &name) : Object(name) { }
        ~Entity() = default;

        friend class Object;
        friend class EntityComponent;
    };

}