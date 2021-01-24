#include "hyppch.hpp"

#include "hyperion/core/object/object.hpp"

#include "hyperion/assets/asset.hpp"
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/assets/shader.hpp"
#include "hyperion/assets/texture.hpp"
#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/world.hpp"
#include "hyperion/entity/components/behaviour.hpp"
#include "hyperion/entity/components/component.hpp"
#include "hyperion/entity/components/rect_transform.hpp"
#include "hyperion/entity/components/transform.hpp"
#include "hyperion/entity/components/physics/box_collider.hpp"
#include "hyperion/entity/components/physics/collider.hpp"
#include "hyperion/entity/components/physics/sphere_collider.hpp"

namespace Hyperion {

    Object *Object::Create() {
        return new Object();
    }

    Object *Object::Create(const String &name) {
        return new Object(name);
    }

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
        for (auto [object_id, object] : s_objects) {
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

// NOTE: This is actually pretty ugly.
// Ideally the types should register themselves in their corresponding .cpp file.
// But unfortunately the linker may or may not optimize these registrations away
// which obviously leads to very annoying bugs.
// So before there is a better solution we just put everything in here.
HYP_REFLECT_REGISTER_BEGIN
{
    Registration<Object>("Object")
        .constructor(select_overload<Object *()>(&Object::Create))(DefaultConstructorPolicy)
        .constructor(select_overload<Object *(const String &)>(&Object::Create))(DefaultConstructorPolicy)
        .property_readonly("id", &Object::m_id)(metadata(Metadata::Serialize, false))
        .property_readonly("guid", &Object::m_guid)(metadata(Metadata::Serialize, true))
        .property("name", &Object::m_name)(metadata(Metadata::Serialize, true));

    {
        Registration<Asset>("Asset")
            .property_readonly("resource_id", &Asset::m_resource_id)(metadata(Metadata::Serialize, false));
        Registration<Material>("Material")
            .constructor(select_overload<Material *()>(&Material::Create))(DefaultConstructorPolicy)
            .constructor(select_overload<Material *(Shader *)>(&Material::Create))(DefaultConstructorPolicy);
        Registration<Mesh>("Mesh")
            .constructor(select_overload<Mesh *()>(&Mesh::Create))(DefaultConstructorPolicy)
            .constructor(select_overload<Mesh *(const MeshData &, const Vector<Rendering::SubMesh> &, bool)>(&Mesh::Create))(DefaultConstructorPolicy);
        Registration<Shader>("Shader")
            .constructor(select_overload<Shader *()>(&Shader::Create))(DefaultConstructorPolicy)
            .constructor(select_overload<Shader *(const Map<Rendering::ShaderStageFlags, String> &)>(&Shader::Create))(DefaultConstructorPolicy);
        Registration<Texture>("Texture");
        Registration<Texture2D>("Texture2D")
            .constructor(select_overload<Texture2D *()>(&Texture2D::Create))(DefaultConstructorPolicy)
            .constructor(select_overload<Texture2D *(uint32, uint32, Rendering::TextureFormat, Rendering::TextureParameters, const Vector<uint8> &, bool)>(&Texture2D::Create))(DefaultConstructorPolicy);
        Registration<TextureCubemap>("TextureCubemap")
            .constructor(select_overload<TextureCubemap *()>(&TextureCubemap::Create))(DefaultConstructorPolicy);
    }

    {
        Registration<Entity>("Entity")
            .constructor(select_overload<Entity *()>(&Entity::Create))(DefaultConstructorPolicy)
            .constructor(select_overload<Entity *(const String &, const Vec3 &, const Quaternion &, Transform *, World *)>(&Entity::Create))(DefaultConstructorPolicy);
        Registration<World>("World")
            .constructor(select_overload<World *()>(&World::Create))(DefaultConstructorPolicy);
        Registration<Behaviour>("Behaviour");
        Registration<Component>("Component");
        Registration<RectTransform>("RectTransform")
            .constructor(select_overload<RectTransform *()>(&RectTransform::Create))(DefaultConstructorPolicy);
        Registration<Transform>("Transform")
            .constructor(select_overload<Transform *()>(&Transform::Create))(DefaultConstructorPolicy);
        Registration<BoxCollider>("BoxCollider")
            .constructor(select_overload<BoxCollider *()>(&BoxCollider::Create))(DefaultConstructorPolicy);
        Registration<Collider>("Collider");
        Registration<SphereCollider>("SphereCollider")
            .constructor(select_overload<SphereCollider *()>(&SphereCollider::Create))(DefaultConstructorPolicy);
    }
}
HYP_REFLECT_REGISTER_END