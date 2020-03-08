#include "hyppch.hpp"

#include "hyperion/entity/object_manager.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/components/transform.hpp"

namespace Hyperion {

    // TODO: The object manager is currently global and not able to differentiate between worlds

    void ObjectManager::Update(f32 delta_time) {
        for (Component *component : s_components_to_update) {
            component->OnUpdate(delta_time);
        }
    }

    void ObjectManager::LateUpdate() {
        for (Object *object : s_objects_to_destroy) {
            DestroyImmediate(object);
        }
        s_objects_to_destroy.clear();
    }

    void ObjectManager::RegisterComponentForUpdate(Component *component) {
        s_components_to_update.push_back(component);
    }

    void ObjectManager::UnregisterComponentForUpdate(Component *component) {
        auto begin = s_components_to_update.begin();
        auto end = s_components_to_update.end();
        if (std::find(begin, end, component) != end) {
            s_components_to_update.erase(std::remove(begin, end, component));
        }
    }

    void ObjectManager::Destroy(Object *object) {
        s_objects_to_destroy.push_back(object);
    }

    void ObjectManager::DestroyImmediate(Object *object) {
        HYP_ASSERT_MESSAGE(object->GetType() != Transform::GetTypeStatic(), "Destroying a transform component is not allowed");

        object->OnDestroy();

        delete object;
    }

}