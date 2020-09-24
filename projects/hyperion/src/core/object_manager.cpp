#include "hyppch.hpp"

#include "hyperion/core/object_manager.hpp"

namespace Hyperion {

    void ObjectManager::LateUpdate() {
        for (Object *object : s_objects_to_destroy) {
            ObjectManager::DestroyImmediate(object);
        }
        s_objects_to_destroy.clear();
    }

    void ObjectManager::Destroy(Object *object) {
        s_objects_to_destroy.insert(object); 
    }

    void ObjectManager::DestroyImmediate(Object *object) {
        object->OnDestroy();

        delete object;
    }

}