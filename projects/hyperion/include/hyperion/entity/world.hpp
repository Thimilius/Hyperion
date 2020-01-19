#pragma once

#include "hyperion/entity/entity.hpp"

namespace Hyperion {

    class World {
    private:
        Vector<Entity *> m_root_entities;
    public:
        const Vector<Entity *> &GetRootEntites() const { return m_root_entities; }
    private:
        World() = default;
        ~World() = default;

        void AddRootEntity(Entity *entity);
        void RemoveRootEntity(Entity *entity);

        friend class Entity;
        friend class WorldManager;
        friend class TransformComponent;
    };

    class WorldManager {
    private:
        // FIXME: This is probably not the right way to force a default active world
        inline static World *s_active_world = new World();

        inline static Vector<World *> s_worlds = { s_active_world };
    public:
        inline static World *GetActiveWorld() { return s_active_world; }
        inline static void SetActiveWorld(World *world) { s_active_world = world; }
    private:
        WorldManager() = delete;
        ~WorldManager() = delete;
    };

}