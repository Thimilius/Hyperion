#include "hyppch.hpp"

#include "hyperion/core/object.hpp"

#include "hyperion/core/object_manager.hpp"
#include "hyperion/entity/components/transform.hpp"

namespace Hyperion {

    Object *Object::Clone(Object *object) {
        if (object) {
            Object *clone = object->CreateClone();
            object->HandleClone(clone);
            return clone;
        } else {
            return nullptr;
        }
    }

    void Object::Destroy(Object *object) {
        if (object && !object->m_destroyed) {
            HYP_ASSERT_MESSAGE(object->GetType() != rttr::type::get<Transform>(), "Destroying a transform component is not allowed!");

            object->m_destroyed = true;
            ObjectManager::Destroy(object);
        }
    }

    Object::Object() {
        m_id = ObjectManager::RegisterObject(this);
    }

    Object::Object(const String &name) : Object() {
        m_name = name;
    }

    void Object::HandleClone(Object *clone) const {
        clone->m_name = m_name;
    }

    Object::~Object() {
        ObjectManager::UnregisterObject(this);
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
        .property("guid", &Object::m_guid)
        .property("name", &Object::m_name);
}
HYP_REFLECT_REGISTER_END
