#pragma once

namespace Hyperion {

    class Entity;
    class MeshRendererComponent;

    class World {
    private:
        Vector<Entity *> m_root_entities;
        Vector<MeshRendererComponent *> m_mesh_renderers;
    public:
        const Vector<Entity *> &GetRootEntites() const { return m_root_entities; }
        const Vector<MeshRendererComponent *> GetMeshRenderers() const { return m_mesh_renderers; }
    private:
        World() = default;
        ~World() = default;

        void AddRootEntity(Entity *entity);
        void RemoveRootEntity(Entity *entity);

        void AddMeshRenderer(MeshRendererComponent *mesh_renderer);
        void RemoveMeshRenderer(MeshRendererComponent *mesh_renderer);

        friend class Entity;
        friend class WorldManager;
        friend class TransformComponent;
        friend class MeshRendererComponent;
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