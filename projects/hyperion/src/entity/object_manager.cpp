#include "hyppch.hpp"

#include "hyperion/entity/object_manager.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/world_manager.hpp"
#include "hyperion/entity/components/transform.hpp"

namespace Hyperion {
    
    void ObjectManager::Update(f32 delta_time) {
        for (auto &[world, components] : s_components_to_update) {
            for (Component *component : components) {
                component->OnUpdate(delta_time);
            }
        }
    }

    void ObjectManager::LateUpdate() {
        for (Object *object : s_objects_to_destroy) {
            DestroyImmediate(object);
        }
        s_objects_to_destroy.clear();
    }

    void ObjectManager::RegisterComponentForUpdate(Component *component) {
        s_components_to_update[component->GetWorld()].push_back(component);
    }

    void ObjectManager::UnregisterComponentForUpdate(Component *component) {
        World *world = component->GetWorld();

        if (s_components_to_update.find(world) != s_components_to_update.end()) {
            Vector<Component *> &components = s_components_to_update.at(world);
            auto begin = components.begin();
            auto end = components.end();
            if (std::find(begin, end, component) != end) {
                components.erase(std::remove(begin, end, component));
            }
        }
    }

    void ObjectManager::Destroy(Object *object) {
        HYP_ASSERT_MESSAGE(object->GetType() != Transform::GetStaticType(), "Destroying a transform component is not allowed");

        s_objects_to_destroy.push_back(object); 
    }

    void ObjectManager::DestroyImmediate(Object *object) {
        object->OnDestroy();

        delete object;
    }

}