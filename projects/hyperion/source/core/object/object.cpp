//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/core/object/object.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset.hpp"
#include "hyperion/assets/font.hpp"
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/assets/shader.hpp"
#include "hyperion/assets/texture.hpp"
#include "hyperion/assets/texture_atlas.hpp"
#include "hyperion/core/image.hpp"
#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/world.hpp"
#include "hyperion/entity/components/behaviour.hpp"
#include "hyperion/entity/components/component.hpp"
#include "hyperion/entity/components/script.hpp"
#include "hyperion/entity/components/transform.hpp"
#include "hyperion/entity/components/physics/box_collider.hpp"
#include "hyperion/entity/components/physics/collider.hpp"
#include "hyperion/entity/components/physics/sphere_collider.hpp"
#include "hyperion/entity/components/rendering/camera.hpp"
#include "hyperion/entity/components/rendering/light.hpp"
#include "hyperion/entity/components/rendering/renderer.hpp"
#include "hyperion/entity/components/rendering/mesh_renderer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

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
        if (object) {
            Object *clone = object->CreateClone();
            object->HandleClone(clone);
            return clone;
        } else {
            return nullptr;
        }
    }

    //--------------------------------------------------------------
    void Object::Destroy(Object *object) {
        if (object && !object->m_is_destroyed) {
            HYP_ASSERT_MESSAGE(object->GetType() != rttr::type::get<Transform>(), "Destroying a transform component is not allowed!");

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
    void Object::HandleClone(Object *clone) const {
        clone->m_name = m_name;
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
        s_objects_to_destroy.insert(object);
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

//--------------------------------------------------------------
// NOTE: This is actually pretty ugly.
// Ideally the types should register themselves in their corresponding .cpp file.
// But unfortunately the linker may or may not optimize these registrations away which obviously leads to very annoying bugs.
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
        Registration<Font>("Font")
            .constructor(select_overload<Font *()>(&Font::Create))(DefaultConstructorPolicy);
        Registration<FontAtlas>("FontAtlas")
            .constructor(select_overload<FontAtlas *()>(&FontAtlas::Create))(DefaultConstructorPolicy)
            .constructor(select_overload<FontAtlas *(Texture2D *, const FontAtlasMap &)>(&FontAtlas::Create))(DefaultConstructorPolicy);
        Registration<Material>("Material")
            .constructor(select_overload<Material *()>(&Material::Create))(DefaultConstructorPolicy)
            .constructor(select_overload<Material *(Shader *)>(&Material::Create))(DefaultConstructorPolicy);
        Registration<Mesh>("Mesh")
            .constructor(select_overload<Mesh *()>(&Mesh::Create))(DefaultConstructorPolicy)
            .constructor(select_overload<Mesh *(const MeshData &, const Vector<Rendering::SubMesh> &, bool)>(&Mesh::Create))(DefaultConstructorPolicy);
        Registration<Shader>("Shader")
            .constructor(select_overload<Shader *()>(&Shader::Create))(DefaultConstructorPolicy)
            .constructor(select_overload<Shader *(const String &)>(&Shader::Create))(DefaultConstructorPolicy);
        Registration<Texture>("Texture");
        Registration<Texture2D>("Texture2D")
            .constructor(select_overload<Texture2D *()>(&Texture2D::Create))(DefaultConstructorPolicy)
            .constructor(select_overload<Texture2D *(uint32, uint32, Rendering::TextureFormat, Rendering::TextureParameters, const Vector<uint8> &, bool)>(&Texture2D::Create))(DefaultConstructorPolicy);
        Registration<TextureCubemap>("TextureCubemap")
            .constructor(select_overload<TextureCubemap *()>(&TextureCubemap::Create))(DefaultConstructorPolicy);
        Registration<RenderTexture>("RenderTexture")
            .constructor(select_overload<RenderTexture *()>(&RenderTexture::Create))(DefaultConstructorPolicy)
            .constructor(select_overload<RenderTexture *(uint32, uint32, Vector<Rendering::RenderTextureAttachment>)>(&RenderTexture::Create))(DefaultConstructorPolicy);
    }

    {
        Registration<Image>("Image")
            .constructor(select_overload<Image *()>(&Image::Create))(DefaultConstructorPolicy)
            .constructor(select_overload<Image *(uint32, uint32, uint32, Vector<uint8> &&)>(&Image::Create))(DefaultConstructorPolicy);
    }

    {
        Registration<Entity>("Entity")
            .constructor(select_overload<Entity *()>(&Entity::Create))(DefaultConstructorPolicy)
            .constructor(select_overload<Entity *(const String &, const Vec3 &, const Quaternion &, Transform *, World *)>(&Entity::Create))(DefaultConstructorPolicy);
        Registration<World>("World")
            .constructor(select_overload<World *()>(&World::Create))(DefaultConstructorPolicy);
        Registration<Behaviour>("Behaviour");
        Registration<Component>("Component");
        Registration<Script>("Script")
            .constructor(select_overload<Script *()>(&Script::Create))(DefaultConstructorPolicy);
        Registration<Transform>("Transform")
            .constructor(select_overload<Transform *()>(&Transform::Create))(DefaultConstructorPolicy);
        Registration<BoxCollider>("BoxCollider")
            .constructor(select_overload<BoxCollider *()>(&BoxCollider::Create))(DefaultConstructorPolicy);
        Registration<Collider>("Collider");
        Registration<SphereCollider>("SphereCollider")
            .constructor(select_overload<SphereCollider *()>(&SphereCollider::Create))(DefaultConstructorPolicy);
        Registration<Camera>("Camera")
            .constructor(select_overload<Camera *()>(&Camera::Create))(DefaultConstructorPolicy);
        Registration<Light>("Light")
            .constructor(select_overload<Light *()>(&Light::Create))(DefaultConstructorPolicy);
        Registration<Renderer>("Renderer");
        Registration<MeshRenderer>("MeshRenderer")
            .constructor(select_overload<MeshRenderer *()>(&MeshRenderer::Create))(DefaultConstructorPolicy);
    }
}
HYP_REFLECT_REGISTER_END
