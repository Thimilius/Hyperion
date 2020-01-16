#pragma once

#include <type_traits>
#include <typeindex>

#include "hyperion/assets/asset.hpp"
#include "hyperion/entity/entity_component.hpp"

namespace Hyperion {

    class Scene;

    class Entity : public Asset {
    private:
        Scene *m_scene;
        Map<std::type_index, EntityComponent *> m_components;

        bool m_destroyed = false;
    public:

        inline AssetType GetType() const override { return AssetType::Entity; }

        inline Scene *GetScene() const { return m_scene; }

        template<class T>
        void AddComponent() {
            // Template constraint
            static_assert(std::is_base_of<EntityComponent, T>::value, "Template parameter must derive from EntityComponent");

            std::type_index type_index = typeid(T);
            // TODO: Should we allow multiple components of the same type?
            HYP_ASSERT_MESSAGE(m_components.find(type_index) == m_components.end(), "Failed to add component because a component with the same type already exists!");

            EntityComponent *component = new T();
            component->m_entity = this;
            m_components[type_index] = component;
        }

        template<class T>
        T *GetComponent() const {
            // Template constraint
            static_assert(std::is_base_of<EntityComponent, T>::value, "Template parameter must derive from EntityComponent");

            std::type_index type_index = typeid(T);
            auto iterator = m_components.find(type_index);
            if (iterator != m_components.end()) {
                return (T*)iterator->second;
            } else {
                return nullptr;
            }
        }

        void Destroy();

        static Entity *Create(const String &name = "New Entity");
    private:
        Entity(const String &name);
        ~Entity() = default;
    };

}