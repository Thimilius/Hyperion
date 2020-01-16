#pragma once

#include <type_traits>
#include <typeindex>

#include "hyperion/entity/components/transform_component.hpp"

namespace Hyperion {

    class Scene;

    class Entity : public Object {
        HYP_OBJECT(Entity, Object);
    private:
        TransformComponent m_transform;
        Map<ObjectType, EntityComponent *> m_components = { { TransformComponent::GetStaticType(), &m_transform } };
        Scene *m_scene;
    public:
        inline Scene *GetScene() const { return m_scene; }
        inline TransformComponent &GetTransform() { return m_transform; }

        template<class T>
        void AddComponent() {
            // Template component constraint
            static_assert(std::is_base_of<EntityComponent, T>::value, "Template parameter must derive from EntityComponent");
            static_assert(!std::is_same<EntityComponent, T>::value, "Template parameter must derive from EntityComponent");
            static_assert(!std::is_base_of<TransformComponent, T>::value, "Can not add an additional TransformComponent");

            ObjectType type = T::GetStaticType();
            // TODO: Should we allow multiple components of the same type?
            HYP_ASSERT_MESSAGE(m_components.find(type) == m_components.end(), "Failed to add component because a component with the same type already exists!");

            EntityComponent *component = new T();
            component->m_entity = this;
            m_components[type] = component;
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

        static Entity *Create(const String &name = "New Entity");
    private:
        Entity(const String &name);
        ~Entity();

        void OnComponentDestroyed(EntityComponent *component);

        friend class Object;
        friend class EntityComponent;
    };

}