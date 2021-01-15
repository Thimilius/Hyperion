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

    ObjectId ObjectManager::RegisterObject(Object *object) {
        // 0 is not a valid object id, so we preincrement
        ObjectId id = ++s_object_id_counter;
        s_objects[id] = object;
        return id;
    }

    void ObjectManager::UnregisterObject(Object *object) {
        HYP_ASSERT(object);

        ObjectId id = object->GetId();

        HYP_ASSERT(s_objects.find(id) != s_objects.end());

        s_objects.erase(id);
    }

}