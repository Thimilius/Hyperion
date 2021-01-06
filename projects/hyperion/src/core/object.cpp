#include "hyppch.hpp"

#include "hyperion/core/object.hpp"

#include "hyperion/core/object_manager.hpp"
#include "hyperion/entity/components/transform.hpp"

namespace Hyperion {

    void Object::Destroy(Object *object) {
        if (object && !object->m_destroyed) {
            HYP_ASSERT_MESSAGE(object->GetType() != rttr::type::get<Transform>(), "Destroying a transform component is not allowed!");

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

HYP_REFLECT_REGISTER_BEGIN
{
    registration::class_<Object>("Object")
        .property("name", &Object::m_name);
}
HYP_REFLECT_REGISTER_END
