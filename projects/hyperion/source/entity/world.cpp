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
        stream.ReadArray("root_entities", context, [this, &context](uint64 index, IArrayReader &reader) {
            SerializableAllocatorFunction allocator = []() { return Type::get<Entity>().create().get_value<Entity *>(); };
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
    World *World::Create() {
        return new World();
    }

}
