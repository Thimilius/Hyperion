#include "hyppch.hpp"

#include "hyperion/entity/object.hpp"

#include "hyperion/entity/object_manager.hpp"

namespace Hyperion {

    void Object::Destroy(Object *object) {
        if (object && !object->m_destroyed) {
            object->m_destroyed = true;
            ObjectManager::Destroy(object);
        }
    }

    void Object::DestroyImmediate(Object *object) {
        if (object && !object->m_destroyed) {
            object->m_destroyed = true;
            ObjectManager::DestroyImmediate(object);
        }
    }

}