//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/entity.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/entity/world_manager.hpp"
#include "hyperion/entity/components/rect_transform.hpp"
#include "hyperion/entity/components/physics/box_collider.hpp"
#include "hyperion/entity/components/physics/sphere_collider.hpp"
#include "hyperion/entity/components/rendering/camera.hpp"
#include "hyperion/entity/components/rendering/mesh_renderer.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    bool Entity::IsActiveInHierarchy() const {
        if (!m_active) {
            return false;
        } else {
            Transform *parent = m_transform->m_parent;
            if (parent != nullptr) {
                return parent->GetEntity()->IsActiveInHierarchy();
            } else {
                return true;
            }
        }
    }

    //--------------------------------------------------------------
    Component *Entity::AddComponent(Type *type) {
        HYP_ASSERT(type);
        HYP_ASSERT(type->IsDerivedFrom<Component>());
        HYP_ASSERT(type != Type::Get<Transform>());
        HYP_ASSERT_MESSAGE(type->HasConstructor(), "Failed to add component because the component type does not have a constructor!");

        Component *component = type->CreateAs<Component>();
        component->m_entity = this;
        m_components.emplace(type, component);

        MakeSureRequiredComponentsArePresent(type);
        
        component->OnCreate();

        return component;
    }

    //--------------------------------------------------------------
    Component *Entity::GetComponent(Type *type) const {
        HYP_ASSERT(type);

        for (auto [component_type, component] : m_components) {
            if (component_type->IsDerivedFrom(type)) {
                return component;
            }
        }

        return nullptr;
    }

    //--------------------------------------------------------------
    Component *Entity::GetComponentInChildren(Type *type) const {
        HYP_ASSERT(type);

        Component *component = GetComponent(type);
        if (component) {
            return component;
        } else {
            for (Transform *child : m_transform->m_children) {
                component = child->GetEntity()->GetComponentInChildren(type);
                if (component) {
                    return component;
                }
            }
        }

        return nullptr;
    }

    //--------------------------------------------------------------
    Component *Entity::GetComponentInParent(Type *type) const {
        HYP_ASSERT(type);

        Component *component = GetComponent(type);
        if (component) {
            return component;
        } else {
            Transform *parent = m_transform->m_parent;
            if (parent != nullptr) {
                return parent->GetEntity()->GetComponentInParent(type);
            }
        }

        return nullptr;
    }

    //--------------------------------------------------------------
    void Entity::DispatchMessage(EntityMessage message) {
        for (IEntityMessageListener *listener : m_message_listeners) {
            listener->OnEntityMessage(message);
        }
    }

    //--------------------------------------------------------------
    void Entity::RegisterMessageListener(IEntityMessageListener *listener) {
        HYP_ASSERT(listener);

        m_message_listeners.push_back(listener);
    }

    //--------------------------------------------------------------
    void Entity::UnregisterMessageListener(IEntityMessageListener *listener) {
        HYP_ASSERT(listener);

        auto begin = m_message_listeners.begin();
        auto end = m_message_listeners.end();
        if (std::find(begin, end, listener) != end) {
            m_message_listeners.erase(std::remove(begin, end, listener));
        }
    }

    //--------------------------------------------------------------
    class ComponentPair : public ISerializable {
    public:
        ComponentPair() = default;
        ComponentPair(const std::pair<Type *, Component *> &pair) {
            m_type = pair.first;
            m_component = pair.second;
        }
    public:
        Type *GetType() const { return m_type; }
        Component *GetComponent() const { return m_component; }

        void Serialize(ISerializationStream &stream) override {
            stream.WriteString("type", m_type->GetName());
            stream.WriteObject("component", m_component);
        }

        void Deserialize(IDeserializationStream &stream) override {
            String type_name = stream.ReadString("type");
            Type *type = Type::GetByName(type_name);
            HYP_ASSERT(type);
            m_type = type;

            m_component = stream.ReadObject<Component>("component", [type]() { return type->CreateAs<Component>(); });
        }
    private:
        Type *m_type = nullptr;
        Component *m_component = nullptr;
    };

    //--------------------------------------------------------------
    void Entity::Serialize(ISerializationStream &stream) {
        Object::Serialize(stream);

        stream.WriteBool("active", m_active);
        stream.WriteUInt32("layer", static_cast<uint32>(m_layer));
        auto tags_iterator = m_tags.begin();
        stream.WriteArray("tags", m_tags.size(), [&tags_iterator](uint64 index, IArrayWriter &writer) {
            writer.WriteString(*tags_iterator++);
        });
        auto components_iterator = m_components.begin();
        stream.WriteArray("components", m_components.size(), [&components_iterator](uint64 index, IArrayWriter &writer) {
            ComponentPair component_pair = *components_iterator++;
            writer.WriteStruct(&component_pair);
        });

        // Children are special and get directly serialized here inside the entity.
        Vector<Transform *> &children = m_transform->m_children;
        stream.WriteArray("children", m_transform->m_children.size(), [&children](uint64 index, IArrayWriter &writer) {
            writer.WriteObject(children[index]->GetEntity());
        });
    }

    //--------------------------------------------------------------
    void Entity::Deserialize(IDeserializationStream &stream) {
        Object::Deserialize(stream);

        m_active = stream.ReadBool("active");
        m_layer = static_cast<LayerMask>(stream.ReadUInt32("layer"));
        stream.ReadArray("tags", [this](uint64 index, IArrayReader &reader) {
            m_tags.insert(reader.ReadString());
        });
        stream.ReadArray("components", [this](uint64 index, IArrayReader &reader) {
            ComponentPair component_pair = reader.ReadStruct<ComponentPair>();
            m_components.emplace(component_pair.GetType(), component_pair.GetComponent());
        });

        for (auto [component_type, component] : m_components) {
            component->m_entity = this;

            if (component_type->IsDerivedFrom<Transform>()) {
                m_transform = static_cast<Transform *>(component);
            }
        }

        Vector<Transform *> &children = m_transform->m_children;
        SerializableAllocatorFunction allocator = []() { return new Entity(); };
        stream.ReadArray("children", [this, &children, &allocator](uint64 index, IArrayReader &reader) {
            Entity *entity = reader.ReadObject<Entity>(allocator);
            children.push_back(entity->GetTransform());
        });
        for (Transform *child : children) {
            child->m_parent = m_transform;
        }
    }

    //--------------------------------------------------------------
    Entity *Entity::Create(const EntityCreationParameters &creation_parameters) {
        Entity *entity = new Entity(creation_parameters.name);

        World *world = creation_parameters.world;
        if (creation_parameters.parent) {
            world = creation_parameters.parent->GetWorld();
        } else if (!world) {
            world = WorldManager::GetActiveWorld();
        }
        HYP_ASSERT(world);
        entity->m_world = world;

        entity->m_transform = new Transform();
        entity->m_transform->m_entity = entity;
        entity->m_transform->m_local_position = creation_parameters.position;
        entity->m_transform->m_derived_position = creation_parameters.position;
        entity->m_transform->m_local_rotation = creation_parameters.rotation;
        entity->m_transform->m_derived_rotation = creation_parameters.rotation;
        entity->m_transform->m_local_scale = creation_parameters.scale;
        entity->m_transform->m_derived_scale = creation_parameters.scale;
        entity->m_transform->OnCreate();
        entity->m_components.emplace(Type::Get<Transform>(), entity->m_transform);

        if (creation_parameters.parent) {
            entity->m_transform->SetParent(creation_parameters.parent);
        } else {
            entity->m_world->AddRootEntity(entity);
        }

        return entity;
    }

    //--------------------------------------------------------------
    Entity *Entity::CreatePrimitive(EntityPrimitive primitive, const EntityCreationParameters &creation_parameters) {
        Entity *entity = Create(creation_parameters);
        entity->SetName(GetPrimitiveName(primitive));
        
        MeshRenderer *mesh_renderer = nullptr;
        switch (primitive) {
            case EntityPrimitive::Quad: {
                mesh_renderer = entity->AddComponent<MeshRenderer>();
                mesh_renderer->SetMesh(AssetManager::GetMeshPrimitive(MeshPrimitive::Quad));
                break;
            }
            case EntityPrimitive::Plane: {
                mesh_renderer = entity->AddComponent<MeshRenderer>();
                mesh_renderer->SetMesh(AssetManager::GetMeshPrimitive(MeshPrimitive::Plane));
                break;
            }
            case EntityPrimitive::Cube: {
                mesh_renderer = entity->AddComponent<MeshRenderer>();
                mesh_renderer->SetMesh(AssetManager::GetMeshPrimitive(MeshPrimitive::Cube));
                entity->AddComponent<BoxCollider>();
                break;
            }
            case EntityPrimitive::Sphere: {
                mesh_renderer = entity->AddComponent<MeshRenderer>();
                mesh_renderer->SetMesh(AssetManager::GetMeshPrimitive(MeshPrimitive::Sphere));
                entity->AddComponent<SphereCollider>();
                break;
            }
            case EntityPrimitive::DirectionalLight: break;
            case EntityPrimitive::PointLight: break;
            case EntityPrimitive::SpotLight: break;
            case EntityPrimitive::Camera: {
                entity->AddComponent<Camera>();
                break;
            }
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE;
        }

        if (mesh_renderer != nullptr) {
            mesh_renderer->SetMaterial(AssetManager::GetMaterialPrimitive(MaterialPrimitive::Default));
        }

        return entity;
    }

    //--------------------------------------------------------------
    void Entity::OnDestroy() {
        // First destroy every child.
        if (!m_transform->m_children.empty()) {
            for (int32 i = (static_cast<int32>(m_transform->m_children.size())) - 1; i >= 0; i--) {
                Entity *child = m_transform->m_children[i]->m_entity;
                // Children already scheduled for destruction get seperated from us by removing the parent reference.
                // This way the order of destruction is not important.
                if (child->IsDestroyed()) {
                    child->m_transform->m_parent = nullptr;
                } else {
                    DestroyImmediate(child);
                }
            }
        }

        // Now destroy all our components except transform.
        for (auto it = m_components.begin(); it != m_components.end(); ) {
            auto [component_type, component] = *it;
            if (component_type->IsDerivedFrom(Type::Get<Transform>())) {
                it++;
            } else {
                it = m_components.erase(it);
                DestroyImmediate(component);
            }
        }

        if (m_transform->GetType() == Type::Get<RectTransform>()) {
            static_cast<RectTransform *>(m_transform)->m_replace_on_destroy = false;
        }
        // At the very end we can destroy the transform.
        DestroyImmediate(m_transform);
    }

    //--------------------------------------------------------------
    void Entity::NotifyActivationChanged() {
        DispatchMessage(EntityMessage::ActivationChanged);

        for (Transform *child : m_transform->m_children) {
            child->GetEntity()->NotifyActivationChanged();
        }
    }

    //--------------------------------------------------------------
    void Entity::MakeSureRequiredComponentsArePresent(Type *type) {
        // We use this function recursively to go up the type hierarchy and bail out when we reach the component base type.
        if (type == Type::Get<Component>()) {
            return;
        }

        const Vector<Metadata> &metadata = type->GetMetadata();
        for (Metadata data : metadata) {
            if (data.kind == MetadataKind::RequireComponent) {
                Type *component_type = data.data.type_pointer;
                Component *required_component = GetComponent(component_type);
                if (required_component == nullptr) {
                    AddComponent(component_type);
                }
            }
        }
        
        // We also want to check the metadata in all our base types.
        // NOTE: We do not use multiple base classes in our code, so we only have one.
        Type *base = type->GetBase();
        if (base != nullptr) {
            MakeSureRequiredComponentsArePresent(base);
        }
    }

    //--------------------------------------------------------------
    void Entity::OnAfterDeserialization() {
        m_transform->OnCreate();
        for (auto [component_type, component] : m_components) {
            if (!component_type->IsDerivedFrom<Transform>()) {
                component->OnCreate();
            }
        }

        for (Transform *child : m_transform->m_children) {
            child->GetEntity()->OnAfterDeserialization();
        }
    }

    //--------------------------------------------------------------
    String Entity::GetPrimitiveName(EntityPrimitive primitive) {
        switch (primitive) {
            case EntityPrimitive::Quad: return "Quad";
            case EntityPrimitive::Plane: return "Plane";
            case EntityPrimitive::Cube: return "Cube";
            case EntityPrimitive::Sphere: return "Sphere";
            case EntityPrimitive::DirectionalLight: return "Directional Light";
            case EntityPrimitive::PointLight: return "Point Light";
            case EntityPrimitive::SpotLight: return "Spot Light";
            case EntityPrimitive::Camera: return "Camera";
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; return "Primitive";
        }
    }

    //--------------------------------------------------------------
    HYP_REFLECT_BEGIN(Entity)
    HYP_REFLECT_BASE(Object)
    HYP_REFLECT_CONSTRUCTOR([]() { return new Entity(); })
    HYP_REFLECT_END()

}
