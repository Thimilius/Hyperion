#pragma once

namespace Hyperion {

    namespace Rendering {
        class TextureCubemap;
    }
    class Entity;
    class MeshRendererComponent;

    class WorldEnvironment {
    private:
        Ref<Rendering::TextureCubemap> m_skybox;

        f32 m_ambient_light_intensity = 0.2f;
        Color m_ambient_light_color = Color::White();
    public:
        f32 GetAmbientLightIntensity() const { return m_ambient_light_intensity; }
        void SetAmbientLightIntensity(f32 ambient_light_intensity) { m_ambient_light_intensity = ambient_light_intensity; }

        Color GetAmbientLightColor() const { return m_ambient_light_color; }
        void SetAmbientLightColor(Color ambient_light_color) { m_ambient_light_color = ambient_light_color; }

        Ref<Rendering::TextureCubemap> GetSkybox() const { return m_skybox; }
        void SetSkybox(const Ref<Rendering::TextureCubemap> skybox) { m_skybox = skybox; }
    };

    class World {
    private:
        WorldEnvironment m_environment;

        Vector<Entity *> m_root_entities;
        Vector<MeshRendererComponent *> m_mesh_renderers;
    public:
        WorldEnvironment &GetEnvironment() { return m_environment; }

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