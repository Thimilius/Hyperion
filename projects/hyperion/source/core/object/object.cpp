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
#include "hyperion/entity/layer.hpp"
#include "hyperion/entity/world.hpp"
#include "hyperion/entity/components/behaviour.hpp"
#include "hyperion/entity/components/component.hpp"
#include "hyperion/entity/components/rect_transform.hpp"
#include "hyperion/entity/components/script.hpp"
#include "hyperion/entity/components/transform.hpp"
#include "hyperion/entity/components/physics/box_collider.hpp"
#include "hyperion/entity/components/physics/collider.hpp"
#include "hyperion/entity/components/physics/sphere_collider.hpp"
#include "hyperion/entity/components/rendering/camera.hpp"
#include "hyperion/entity/components/rendering/light.hpp"
#include "hyperion/entity/components/rendering/mesh_renderer.hpp"
#include "hyperion/entity/components/rendering/renderer.hpp"
#include "hyperion/entity/components/rendering/widget_renderer.hpp"
#include "hyperion/entity/components/ui/canvas.hpp"
#include "hyperion/entity/components/ui/graphic.hpp"
#include "hyperion/entity/components/ui/widget.hpp"
#include "hyperion/entity/components/ui/text.hpp"

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
    RegistrationClass<Object>("Object")
        .constructor(select_overload<Object *()>(&Object::Create))(PointerConstructorPolicy)
        .constructor(select_overload<Object *(const String &)>(&Object::Create))(PointerConstructorPolicy)
        .property("id", &Object::m_id)(metadata(Metadata::DontSerialize, true))
        .property("guid", &Object::m_guid)
        .property("name", &Object::m_name);

    {
        RegistrationClass<Asset>("Asset")
            .property_readonly("resource_id", &Asset::m_resource_id)(metadata(Metadata::DontSerialize, true));
        RegistrationClass<Font>("Font")
            .constructor(select_overload<Font *()>(&Font::Create))(PointerConstructorPolicy);
        RegistrationClass<FontAtlas>("FontAtlas")
            .constructor(select_overload<FontAtlas *()>(&FontAtlas::Create))(PointerConstructorPolicy)
            .constructor(select_overload<FontAtlas *(Texture2D *, const FontAtlasMap &)>(&FontAtlas::Create))(PointerConstructorPolicy);
        RegistrationClass<Material>("Material")
            .constructor(select_overload<Material *()>(&Material::Create))(PointerConstructorPolicy)
            .constructor(select_overload<Material *(Shader *)>(&Material::Create))(PointerConstructorPolicy);
        RegistrationClass<Mesh>("Mesh")
            .constructor(select_overload<Mesh *()>(&Mesh::Create))(PointerConstructorPolicy)
            .constructor(select_overload<Mesh *(const MeshData &, const Vector<Rendering::SubMesh> &, bool)>(&Mesh::Create))(PointerConstructorPolicy);
        RegistrationClass<Shader>("Shader")
            .constructor(select_overload<Shader *()>(&Shader::Create))(PointerConstructorPolicy)
            .constructor(select_overload<Shader *(const String &)>(&Shader::Create))(PointerConstructorPolicy);
        RegistrationClass<Texture>("Texture");
        RegistrationClass<Texture2D>("Texture2D")
            .constructor(select_overload<Texture2D *()>(&Texture2D::Create))(PointerConstructorPolicy)
            .constructor(select_overload<Texture2D *(uint32, uint32, Rendering::TextureFormat, Rendering::TextureParameters, const Vector<uint8> &, bool)>(&Texture2D::Create))(PointerConstructorPolicy);
        RegistrationClass<TextureCubemap>("TextureCubemap")
            .constructor(select_overload<TextureCubemap *()>(&TextureCubemap::Create))(PointerConstructorPolicy);
        RegistrationClass<RenderTexture>("RenderTexture")
            .constructor(select_overload<RenderTexture *()>(&RenderTexture::Create))(PointerConstructorPolicy)
            .constructor(select_overload<RenderTexture *(uint32, uint32, Vector<Rendering::RenderTextureAttachment>)>(&RenderTexture::Create))(PointerConstructorPolicy);
    }

    {
        RegistrationClass<Image>("Image")
            .constructor(select_overload<Image *()>(&Image::Create))(PointerConstructorPolicy)
            .constructor(select_overload<Image *(uint32, uint32, uint32, Vector<uint8> &&)>(&Image::Create))(PointerConstructorPolicy);
    }

    {
        RegistrationClass<Entity>("Entity")
            .constructor(select_overload<Entity *()>(&Entity::Create))(PointerConstructorPolicy)
            .constructor(select_overload<Entity *(const String &, const Vec3 &, const Quaternion &, Transform *, World *)>(&Entity::Create))(PointerConstructorPolicy)
            .property("active", &Entity::m_active)
            .property("layer", &Entity::m_layer);
        Registration::enumeration<LayerMask>("Layer")(
            value("Nothing", LayerMask::Nothing),

            value("Layer1", LayerMask::Layer1),
            value("Layer2", LayerMask::Layer2),
            value("Layer3", LayerMask::Layer3),
            value("Layer4", LayerMask::Layer4),
            value("Layer5", LayerMask::Layer5),
            value("Layer6", LayerMask::Layer6),
            value("Layer7", LayerMask::Layer7),
            value("Layer8", LayerMask::Layer8),
            value("Layer9", LayerMask::Layer9),
            value("Layer10", LayerMask::Layer10),
            value("Layer11", LayerMask::Layer11),
            value("Layer12", LayerMask::Layer12),
            value("Layer13", LayerMask::Layer13),
            value("Layer14", LayerMask::Layer14),
            value("Layer15", LayerMask::Layer15),
            value("Layer16", LayerMask::Layer16),
            value("Layer17", LayerMask::Layer17),
            value("Layer18", LayerMask::Layer18),
            value("Layer19", LayerMask::Layer19),
            value("Layer20", LayerMask::Layer20),
            value("Layer21", LayerMask::Layer21),
            value("Layer22", LayerMask::Layer22),
            value("Layer23", LayerMask::Layer23),
            value("Layer24", LayerMask::Layer24),
            value("Layer25", LayerMask::Layer25),
            value("Layer26", LayerMask::Layer26),
            value("Layer27", LayerMask::Layer27),
            value("Layer28", LayerMask::Layer28),
            value("Layer29", LayerMask::Layer29),
            value("Layer30", LayerMask::Layer30),
            value("Layer31", LayerMask::Layer31),

            value("Default", LayerMask::Default),
            value("Everything", LayerMask::Everything),

            metadata(Metadata::Flags, true)
        );
        RegistrationClass<WorldEnvironment>("WorldEnvironment")
            .constructor()(PointerConstructorPolicy)
            .property("ambient_light", &WorldEnvironment::ambient_light);
        RegistrationClass<WorldEnvironment::AmbientLight>("WorldEnvironment.AmbientLight")
            .constructor()
            .property("intensity", &WorldEnvironment::AmbientLight::intensity)
            .property("color", &WorldEnvironment::AmbientLight::color);
        RegistrationClass<World>("World")
            .constructor(select_overload<World *()>(&World::Create))(PointerConstructorPolicy)
            .property("name", &World::m_name)
            .property("environment", &World::m_environment)
            .property("root_entities", &World::m_root_entities);

        RegistrationClass<Behaviour>("Behaviour");
        RegistrationClass<Component>("Component");
        RegistrationClass<RectTransform>("RectTransform")
            .constructor(select_overload<RectTransform *()>(&RectTransform::Create))(PointerConstructorPolicy);
        RegistrationClass<Script>("Script")
            .constructor(select_overload<Script *()>(&Script::Create))(PointerConstructorPolicy);
        RegistrationClass<Transform>("Transform")
            .constructor(select_overload<Transform *()>(&Transform::Create))(PointerConstructorPolicy);

        RegistrationClass<BoxCollider>("BoxCollider")
            .constructor(select_overload<BoxCollider *()>(&BoxCollider::Create))(PointerConstructorPolicy);
        RegistrationClass<Collider>("Collider");
        RegistrationClass<SphereCollider>("SphereCollider")
            .constructor(select_overload<SphereCollider *()>(&SphereCollider::Create))(PointerConstructorPolicy);

        RegistrationClass<Camera>("Camera")
            .constructor(select_overload<Camera *()>(&Camera::Create))(PointerConstructorPolicy);
        RegistrationClass<Light>("Light")
            .constructor(select_overload<Light *()>(&Light::Create))(PointerConstructorPolicy);
        RegistrationClass<Renderer>("Renderer");
        RegistrationClass<MeshRenderer>("MeshRenderer")
            .constructor(select_overload<MeshRenderer *()>(&MeshRenderer::Create))(PointerConstructorPolicy);
        RegistrationClass<WidgetRenderer>("WidgetRenderer")(metadata(Metadata::RequiresComponent0, Type::get<RectTransform>()))
            .constructor(select_overload<WidgetRenderer *()>(&WidgetRenderer::Create))(PointerConstructorPolicy);

        RegistrationClass<Canvas>("Canvas")(metadata(Metadata::RequiresComponent0, Type::get<RectTransform>()))
            .constructor(select_overload<Canvas *()>(&Canvas::Create))(PointerConstructorPolicy);
        RegistrationClass<Graphic>("Graphic")
            .constructor(select_overload<Graphic *()>(&Graphic::Create))(PointerConstructorPolicy);
        RegistrationClass<Widget>("Widget")(metadata(Metadata::RequiresComponent0, Type::get<RectTransform>()))(metadata(Metadata::RequiresComponent1, Type::get<WidgetRenderer>()));
        RegistrationClass<Text>("Text")
            .constructor(select_overload<Text *()>(&Text::Create))(PointerConstructorPolicy);
    }
}
HYP_REFLECT_REGISTER_END
