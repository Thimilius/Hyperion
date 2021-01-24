#include "hyppch.hpp"

#include "hyperion/core/object/object.hpp"

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

    Object *ObjectManager::Get(ObjectId object_id) {
        HYP_ASSERT(s_objects.find(object_id) != s_objects.end());
        return s_objects[object_id];
    }

    void ObjectManager::LateUpdate() {
        DestroyPendingObjects();
    }

    void ObjectManager::Shutdown() {
        for (auto &[object_id, object] : s_objects) {
            Object::Destroy(object);
        }
        DestroyPendingObjects();
    }

    void ObjectManager::Destroy(Object *object) {
        s_objects_to_destroy.insert(object);
    }

    void ObjectManager::DestroyImmediate(Object *object) {
        object->OnDestroy();

        delete object;
    }

    void ObjectManager::DestroyPendingObjects() {
        for (Object *object : s_objects_to_destroy) {
            ObjectManager::DestroyImmediate(object);
        }
        s_objects_to_destroy.clear();
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

HYP_REFLECT_REGISTER_BEGIN
{
    registration::class_<Object>("Object")
        .property_readonly("guid", &Object::m_guid)
        .property("name", &Object::m_name);
}
HYP_REFLECT_REGISTER_END
