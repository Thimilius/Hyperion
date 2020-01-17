#pragma once

#include <type_traits>
#include <typeindex>

#include "hyperion/entity/entity_event.hpp"
#include "hyperion/entity/components/transform_component.hpp"

namespace Hyperion {

    class Scene;

    // TODO: Implement an entity system to better support entity creation/destruction

    class Entity : public Object, public EntityEventListener {
        HYP_OBJECT(Entity, Object);
    private:
        TransformComponent m_transform;
        Map<ObjectType, EntityComponent *> m_components;
        
        Scene *m_scene;

        Vector<EntityEventListener *> m_event_listeners;
    public:
        inline Scene *GetScene() const { return m_scene; }
        inline TransformComponent *GetTransform() { return &m_transform; }

        template<class T>
        T *AddComponent() {
            // Template component constraint
            static_assert(std::is_base_of<EntityComponent, T>::value, "Template parameter must derive from EntityComponent");
            static_assert(!std::is_same<EntityComponent, T>::value, "Template parameter must derive from EntityComponent");
            static_assert(!std::is_base_of<TransformComponent, T>::value, "Can not add an additional TransformComponent");

            ObjectType type = T::GetStaticType();
            // TODO: Should we allow multiple components of the same type?
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

        void OnEvent(EntityEvent event) override;

        inline void RegisterEventListener(EntityEventListener *listener) {
            m_event_listeners.push_back(listener);
        }

        inline void UnregisterEventListener(EntityEventListener *listener) {
            auto begin = m_event_listeners.begin();
            auto end = m_event_listeners.end();
            if (std::find(begin, end, listener) != end) {
                m_event_listeners.erase(std::remove(begin, end, listener));
            }
        }

        static Entity *Create(const String &name = "New Entity");
    private:
        Entity(const String &name);
        ~Entity();

        friend class Object;
        friend class EntityComponent;
    };

}