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
        m_root_entities.push_back(entity);
    }

    void World::RemoveRootEntity(Entity *entity) {
        auto begin = m_root_entities.begin();
        auto end = m_root_entities.end();
        if (std::find(begin, end, entity) != end) {
            m_root_entities.erase(std::remove(begin, end, entity));
        }
    }

    World *World::Create() {
        return new World();
    }

}
