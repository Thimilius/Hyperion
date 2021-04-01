//----------------- Precompiled Header Include -----------------
#include "hyppch.hpp"

//--------------------- Definition Include ---------------------
#include "hyperion/entity/world.hpp"

//---------------------- Project Includes ----------------------
#include "hyperion/entity/entity.hpp"
#include "hyperion/physics/physics_engine.hpp"

//-------------------- Definition Namespace --------------------
namespace Hyperion {

    //--------------------------------------------------------------
    void WorldEnvironment::AmbientLight::Serialize(ISerializationStream &stream) {
        stream.WriteFloat("intensity", intensity);
        stream.WriteColor("color", color);
    }

    //--------------------------------------------------------------
    void WorldEnvironment::AmbientLight::Deserialize(IDeserializationStream &stream, ReferenceContext &context) {
        intensity = stream.ReadFloat("intensity");
        color = stream.ReadColor("color");
    }

    //--------------------------------------------------------------
    void WorldEnvironment::Serialize(ISerializationStream &stream) {
        stream.WriteStruct("ambient_light", ambient_light);
    }

    //--------------------------------------------------------------
    void WorldEnvironment::Deserialize(IDeserializationStream &stream, ReferenceContext &context) {
        ambient_light = stream.ReadStruct<AmbientLight>("ambient_light", context);
    }

    //--------------------------------------------------------------
    void World::Serialize(ISerializationStream &stream) {
        Object::Serialize(stream);

        stream.WriteStruct("environment", m_environment);
        stream.WriteArray("root_entities", m_root_entities.size(), [this](uint64 index, IArrayWriter &writer) {
            writer.WriteObject(m_root_entities[index]);
        });
    }

    //--------------------------------------------------------------
    void World::Deserialize(IDeserializationStream &stream, ReferenceContext &context) {
        Object::Deserialize(stream, context);
        
        m_environment = stream.ReadStruct<WorldEnvironment>("environment", context);
        SerializableAllocatorFunction allocator = []() { return Type::Get<Entity>()->CreateAs<Entity>(); };
        stream.ReadArray("root_entities", context, [this, &context, &allocator](uint64 index, IArrayReader &reader) {
            m_root_entities.push_back(reader.ReadObject<Entity>(context, allocator));
        });
    }

    //--------------------------------------------------------------
    World::World(const String &name) : Object(name) {
        m_physics_world = Physics::PhysicsEngine::CreatePhysicsWorld();
    }

    //--------------------------------------------------------------
    void World::OnDestroy() {
        for (auto it = m_root_entities.begin(); it != m_root_entities.end(); ) {
            Entity *entity = *it;
            it = m_root_entities.erase(it);
            Object::DestroyImmediate(entity);
        }

        Physics::PhysicsEngine::DestroyPhysicsWorld(m_physics_world);
    }

    //--------------------------------------------------------------
    void World::OnUpdate(float32 delta_time) {
        for (Component *component : m_components_to_update) {
            component->OnUpdate(delta_time);
        }
    }

    //--------------------------------------------------------------
    void World::OnAfterDeserialization() {
        // Before we call the callback on the entites themselves, we first have to set the m_world references of all entites.
        for (Entity *entity : m_root_entities) {
            SetWorldReferenceRecursive(entity);
        }

        for (Entity *entity : m_root_entities) {
            entity->OnAfterDeserialization();
        }
    }

    //--------------------------------------------------------------
    void World::SetWorldReferenceRecursive(Entity *entity) {
        entity->m_world = this;

        for (uint32 i = 0; i < entity->GetTransform()->GetChildCount(); i++) {
            Entity *child_entity = entity->GetTransform()->GetChild(i)->GetEntity();
            SetWorldReferenceRecursive(child_entity);
        }
    }

    //--------------------------------------------------------------
    void World::AddRootEntity(Entity *entity) {
        HYP_ASSERT(entity);
        HYP_ASSERT(std::find(m_root_entities.begin(), m_root_entities.end(), entity) == m_root_entities.end());

        m_root_entities.push_back(entity);
    }

    //--------------------------------------------------------------
    void World::RemoveRootEntity(Entity *entity) {
        HYP_ASSERT(entity);

        auto begin = m_root_entities.begin();
        auto end = m_root_entities.end();
        if (std::find(begin, end, entity) != end) {
            m_root_entities.erase(std::remove(begin, end, entity));
        }
    }

    //--------------------------------------------------------------
    void World::RegisterComponentForUpdate(Component *component) {
        HYP_ASSERT(component);
        HYP_ASSERT(std::find(m_components_to_update.begin(), m_components_to_update.end(), component) == m_components_to_update.end());

        m_components_to_update.push_back(component);
    }

    //--------------------------------------------------------------
    void World::UnregisterComponentForUpdate(Component *component) {
        HYP_ASSERT(component);

        auto begin = m_components_to_update.begin();
        auto end = m_components_to_update.end();
        if (std::find(begin, end, component) != end) {
            m_components_to_update.erase(std::remove(begin, end, component));
        }
    }

    //--------------------------------------------------------------
    void World::AddLight(Light *light) {
        HYP_ASSERT(light);
        HYP_ASSERT(std::find(m_lights.begin(), m_lights.end(), light) == m_lights.end());

        m_lights.push_back(light);
    }

    //--------------------------------------------------------------
    void World::RemoveLight(Light *light) {
        HYP_ASSERT(light);
        HYP_ASSERT(std::find(m_lights.begin(), m_lights.end(), light) != m_lights.end());

        auto begin = m_lights.begin();
        auto end = m_lights.end();
        if (std::find(begin, end, light) != end) {
            m_lights.erase(std::remove(begin, end, light));
        }
    }

    //--------------------------------------------------------------
    void World::AddMeshRenderer(MeshRenderer *mesh_renderer) {
        HYP_ASSERT(mesh_renderer);
        HYP_ASSERT(std::find(m_mesh_renderers.begin(), m_mesh_renderers.end(), mesh_renderer) == m_mesh_renderers.end());

        m_mesh_renderers.push_back(mesh_renderer);
    }

    //--------------------------------------------------------------
    void World::RemoveMeshRenderer(MeshRenderer *mesh_renderer) {
        HYP_ASSERT(mesh_renderer);
        HYP_ASSERT(std::find(m_mesh_renderers.begin(), m_mesh_renderers.end(), mesh_renderer) != m_mesh_renderers.end());

        auto begin = m_mesh_renderers.begin();
        auto end = m_mesh_renderers.end();
        if (std::find(begin, end, mesh_renderer) != end) {
            m_mesh_renderers.erase(std::remove(begin, end, mesh_renderer));
        }
    }

    //--------------------------------------------------------------
    HYP_REFLECT_BEGIN(World)
    HYP_REFLECT_BASE(Object)
    HYP_REFLECT_CONSTRUCTOR([]() { return new World(); })
    HYP_REFLECT_END()

}
