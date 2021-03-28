//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/object/object.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/components/transform.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void Object::Serialize(ISerializationStream &stream) {
        stream.WriteString("guid", m_guid.ToString());
        stream.WriteString("name", m_name);
    }

    //--------------------------------------------------------------
    void Object::Deserialize(IDeserializationStream &stream, ReferenceContext &context) {
        m_guid = Guid::Create(stream.ReadString("guid"));
        m_name = stream.ReadString("name");
        
        context.Register(m_guid, this);
    }

    //--------------------------------------------------------------
    Object *Object::Create() {
        return new Object();
    }

    //--------------------------------------------------------------
    Object *Object::Create(const String &name) {
        return new Object(name);
    }

    //--------------------------------------------------------------
    Object *Object::Clone(Object *object) {
        HYP_ASSERT(object);

        // TODO: Implement.
        return nullptr;
    }

    //--------------------------------------------------------------
    void Object::Destroy(Object *object) {
        // NOTE: Currently we do not do any kind of validation here if we are actually allowed to destroy the object.

        if (object && !object->m_is_destroyed) {
            object->m_is_destroyed = true;
            ObjectManager::Destroy(object);
        }
    }

    //--------------------------------------------------------------
    Object::Object() {
        m_id = ObjectManager::RegisterObject(this);
    }

    //--------------------------------------------------------------
    Object::Object(const String &name) : Object() {
        m_name = name;
    }

    //--------------------------------------------------------------
    void Object::OnClone(Object *clone) {
        clone->m_name = m_name;

        // Guid and id do not need to be copied as they are distinct for each object.
        // The scripting instance does also not need to be copied as we have a completely new native object that needs to get handled by the scripting driver.
    }

    //--------------------------------------------------------------
    Object::~Object() {
        ObjectManager::UnregisterObject(this);
    }

    //--------------------------------------------------------------
    void Object::DestroyImmediate(Object *object) {
        if (object && !object->m_is_destroyed) {
            object->m_is_destroyed = true;
            ObjectManager::DestroyImmediate(object);
        }
    }

    //--------------------------------------------------------------
    Object *ObjectManager::Get(ObjectId object_id) {
        auto it = s_objects.find(object_id);
        if (it != s_objects.end()) {
            return it->second;
        } else {
            return nullptr;
        }
    }

    //--------------------------------------------------------------
    HYP_REFLECT_BEGIN(Object)
    HYP_REFLECT_BASE(Scripting::ScriptingObject)
    HYP_REFLECT_END()

    //--------------------------------------------------------------
    void ObjectManager::LateUpdate() {
        DestroyPendingObjects();
    }

    //--------------------------------------------------------------
    void ObjectManager::Shutdown() {
        for (auto [object_id, object] : s_objects) {
            Object::Destroy(object);
        }
        DestroyPendingObjects();
    }

    //--------------------------------------------------------------
    void ObjectManager::Destroy(Object *object) {
        s_objects_to_destroy.push_back(object);
    }

    //--------------------------------------------------------------
    void ObjectManager::DestroyImmediate(Object *object) {
        object->OnDestroy();

        delete object;
    }

    //--------------------------------------------------------------
    void ObjectManager::DestroyPendingObjects() {
        for (Object *object : s_objects_to_destroy) {
            ObjectManager::DestroyImmediate(object);
        }
        s_objects_to_destroy.clear();
    }

    //--------------------------------------------------------------
    ObjectId ObjectManager::RegisterObject(Object *object) {
        // 0 is not a valid object id, so we preincrement.
        ObjectId id = ++s_object_id_counter;
        s_objects[id] = object;
        return id;
    }

    //--------------------------------------------------------------
    void ObjectManager::UnregisterObject(Object *object) {
        HYP_ASSERT(object);

        ObjectId id = object->GetId();

        HYP_ASSERT(s_objects.find(id) != s_objects.end());

        s_objects.erase(id);
    }

}
