#include "hyppch.hpp"

#include "hyperion/entity/world.hpp"

#include "hyperion/entity/entity.hpp"
#include "hyperion/physics/physics_engine.hpp"

namespace Hyperion {

    World::World() {
        m_physics_world = Physics::PhysicsEngine::CreatePhysicsWorld();
    }

    World::~World() {
        for (Entity *entity : m_root_entities) {
            Object::DestroyImmediate(entity);
        }

        Physics::PhysicsEngine::DestroyPhysicsWorld(m_physics_world);
    }

    void World::AddRootEntity(Entity *entity) {
        m_root_entities.push_back(entity);
    }

    void World::RemoveRootEntity(Entity *entity) {
        auto begin = m_root_entities.begin();
        auto end = m_root_entities.end();
        if (std::find(begin, end, entity) != end) {
            m_root_entities.erase(std::remove(begin, end, entity));
        }
    }

    void World::AddCamera(Camera *camera) {
        m_cameras.push_back(camera);
    }

    void World::RemoveCamera(Camera *camera) {
        auto begin = m_cameras.begin();
        auto end = m_cameras.end();
        if (std::find(begin, end, camera) != end) {
            m_cameras.erase(std::remove(begin, end, camera));
        }
    }

    void World::AddMeshRenderer(MeshRenderer *mesh_renderer) {
        m_mesh_renderers.push_back(mesh_renderer);
    }

    void World::RemoveMeshRenderer(MeshRenderer *mesh_renderer) {
        auto begin = m_mesh_renderers.begin();
        auto end = m_mesh_renderers.end();
        if (std::find(begin, end, mesh_renderer) != end) {
            m_mesh_renderers.erase(std::remove(begin, end, mesh_renderer));
        }
    }

    void World::AddLight(Light *light) {
        m_lights.push_back(light);
    }

    void World::RemoveLight(Light *light) {
        auto begin = m_lights.begin();
        auto end = m_lights.end();
        if (std::find(begin, end, light) != end) {
            m_lights.erase(std::remove(begin, end, light));
        }
    }

}