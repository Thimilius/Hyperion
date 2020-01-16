#pragma once

#include <type_traits>
#include <typeindex>

#include "hyperion/assets/asset.hpp"
#include "hyperion/entity/components/transform_component.hpp"

namespace Hyperion {

    class Scene;

    class Entity : public Asset {
    private:
        TransformComponent m_transform;
        Map<std::type_index, EntityComponent *> m_components = { { typeid(TransformComponent), &m_transform } };
        Scene *m_scene;

        bool m_destroyed = false;
    public:
        inline AssetType GetType() const override { return AssetType::Entity; }

        inline Scene *GetScene() const { return m_scene; }
        inline TransformComponent &GetTransform() { return m_transform; }

        template<class T>
        void AddComponent() {
            // Template component constraint
            static_assert(std::is_base_of<EntityComponent, T>::value, "Template parameter must derive from EntityComponent");
            static_assert(!std::is_same<EntityComponent, T>::value, "Template parameter must derive from EntityComponent");
            static_assert(!std::is_base_of<TransformComponent, T>::value, "Can not add an additional TransformComponent");

            std::type_index type_index = typeid(T);
            // TODO: Should we allow multiple components of the same type?
            HYP_ASSERT_MESSAGE(m_components.find(type_index) == m_components.end(), "Failed to add component because a component with the same type already exists!");

            EntityComponent *component = new T();
            component->m_entity = this;
            m_components[type_index] = component;
        }

        template<class T>
        T *GetComponent() const {
            // Template component constraint
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

        static void Destroy(Entity *entity);
        static Entity *Create(const String &name = "New Entity");
    private:
        Entity(const String &name);
        ~Entity() = default;

        // Entities can not be copied
        Entity(Entity &other) = delete;
        Entity operator=(Entity &other) = delete;
    };

}