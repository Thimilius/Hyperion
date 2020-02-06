#include "hyppch.hpp"

#include "hyperion/entity/world.hpp"

namespace Hyperion {

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

    void World::AddMeshRenderer(MeshRendererComponent *mesh_renderer) {
        m_mesh_renderers.push_back(mesh_renderer);
    }

    void World::RemoveMeshRenderer(MeshRendererComponent *mesh_renderer) {
        auto begin = m_mesh_renderers.begin();
        auto end = m_mesh_renderers.end();
        if (std::find(begin, end, mesh_renderer) != end) {
            m_mesh_renderers.erase(std::remove(begin, end, mesh_renderer));
        }
    }

    void World::AddLight(LightComponent *light) {
        m_lights.push_back(light);
    }

    void World::RemoveLight(LightComponent *light) {
        auto begin = m_lights.begin();
        auto end = m_lights.end();
        if (std::find(begin, end, light) != end) {
            m_lights.erase(std::remove(begin, end, light));
        }
    }

}