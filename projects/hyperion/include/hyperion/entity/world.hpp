#pragma once

namespace Hyperion {

    namespace Rendering {
        class TextureCubemap;
    }
    class Entity;
    class MeshRenderer;
    class Light;

    enum class WorldEnvironmentBackgroundMode {
        Color,
        Skybox
    };

    struct WorldEnvironment {
        WorldEnvironmentBackgroundMode background_mode = WorldEnvironmentBackgroundMode::Color;
        Color background_color = Color::Black();
        Ref<Rendering::TextureCubemap> background_skybox;

        f32 ambient_light_intensity = 0.2f;
        Color ambient_light_color = Color::White();
    };

    class World {
    private:
        WorldEnvironment m_environment;

        Vector<Entity *> m_root_entities;
        Vector<MeshRenderer *> m_mesh_renderers;
        Vector<Light *> m_lights;
    public:
        inline WorldEnvironment &GetEnvironment() { return m_environment; }

        inline const Vector<Entity *> &GetRootEntites() const { return m_root_entities; }
        inline const Vector<MeshRenderer *> GetMeshRenderers() const { return m_mesh_renderers; }
        inline const Vector<Light *> GetLights() const { return m_lights; }
    private:
        World() = default;
        ~World() = default;

        void AddRootEntity(Entity *entity);
        void RemoveRootEntity(Entity *entity);

        void AddMeshRenderer(MeshRenderer *mesh_renderer);
        void RemoveMeshRenderer(MeshRenderer *mesh_renderer);

        void AddLight(Light *light);
        void RemoveLight(Light *light);

        friend class Entity;
        friend class WorldManager;
        friend class Transform;
        friend class MeshRenderer;
        friend class Light;
    };

    class WorldManager {
    private:
        inline static World *s_active_world;

        inline static Vector<World *> s_worlds;
    public:
        inline static World *GetActiveWorld() { return s_active_world; }
        inline static void SetActiveWorld(World *world) { s_active_world = world; }

        inline static World *CreateWorld() {
            World *world = new World();
            s_worlds.push_back(world);
            return world;
        }
        // TODO: Add ability to destroy world
    private:
        WorldManager() = delete;
        ~WorldManager() = delete;
    };

}