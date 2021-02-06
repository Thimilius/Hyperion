#include "hyppch.hpp"

#include "hyperion/entity/world.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/physics/physics_engine.hpp"

namespace Hyperion {

    void World::OnDestroy() {
        for (auto it = m_root_entities.begin(); it != m_root_entities.end(); ) {
            Entity *entity = *it;
            it = m_root_entities.erase(it);
            Object::DestroyImmediate(entity);
        }

        Physics::PhysicsEngine::DestroyPhysicsWorld(m_physics_world);
    }

    World::World(const String &name) {
        m_physics_world = Physics::PhysicsEngine::CreatePhysicsWorld();
    }
    
    void World::AddRootEntity(Entity *entity) {
        HYP_ASSERT(entity);
        HYP_ASSERT(std::find(m_root_entities.begin(), m_root_entities.end(), entity) == m_root_entities.end());
        
        m_root_entities.push_back(entity);
    }

    void World::RemoveRootEntity(Entity *entity) {
        HYP_ASSERT(entity);

        auto begin = m_root_entities.begin();
        auto end = m_root_entities.end();
        if (std::find(begin, end, entity) != end) {
            m_root_entities.erase(std::remove(begin, end, entity));
        }
    }

    void World::AddMeshRenderer(MeshRenderer *mesh_renderer) {
        HYP_ASSERT(mesh_renderer);
        HYP_ASSERT(std::find(m_mesh_renderers.begin(), m_mesh_renderers.end(), mesh_renderer) == m_mesh_renderers.end());

        m_mesh_renderers.push_back(mesh_renderer);
    }

    void World::RemoveMeshRenderer(MeshRenderer *mesh_renderer) {
        HYP_ASSERT(mesh_renderer);
        HYP_ASSERT(std::find(m_mesh_renderers.begin(), m_mesh_renderers.end(), mesh_renderer) != m_mesh_renderers.end());

        auto begin = m_mesh_renderers.begin();
        auto end = m_mesh_renderers.end();
        if (std::find(begin, end, mesh_renderer) != end) {
            m_mesh_renderers.erase(std::remove(begin, end, mesh_renderer));
        }
    }

    World *World::Create() {
        return new World();
    }

}
