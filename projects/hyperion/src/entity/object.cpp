#include "hyppch.hpp"

#include "hyperion/entity/object.hpp"

#include "hyperion/entity/object_manager.hpp"

namespace Hyperion {

    void Object::Destroy(Object *object) {
        if (object && !object->m_destroyed) {
            ObjectManager::Destroy(object);
            object->m_destroyed = true;
        }
    }

    void Object::DestroyImmediate(Object *object) {
        if (object && !object->m_destroyed) {
            ObjectManager::DestroyImmediate(object);
        }
    }

}