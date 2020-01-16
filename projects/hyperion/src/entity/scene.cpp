#include "hyppch.hpp"

#include "hyperion/entity/scene.hpp"

namespace Hyperion {

    void Scene::AddRootEntity(Entity *entity) {
        m_root_entities.push_back(entity);
    }

    void Scene::OnEntityDestroyed(Entity *entity) {
        auto begin = m_root_entities.begin();
        auto end = m_root_entities.end();
        if (std::find(begin, end, entity) != end) {
            m_root_entities.erase(std::remove(begin, end, entity), end);
        }
    }

}