#pragma once

namespace Hyperion {

    namespace Rendering {
        class TextureCubemap;
    }
    class Entity;
    class MeshRendererComponent;
    class LightComponent;

    class WorldEnvironment {
    private:
        Ref<Rendering::TextureCubemap> m_skybox;

        f32 m_ambient_light_intensity = 0.2f;
        Color m_ambient_light_color = Color::White();
    public:
        inline f32 GetAmbientLightIntensity() const { return m_ambient_light_intensity; }
        inline void SetAmbientLightIntensity(f32 ambient_light_intensity) { m_ambient_light_intensity = ambient_light_intensity; }

        inline Color GetAmbientLightColor() const { return m_ambient_light_color; }
        inline void SetAmbientLightColor(Color ambient_light_color) { m_ambient_light_color = ambient_light_color; }

        inline Ref<Rendering::TextureCubemap> GetSkybox() const { return m_skybox; }
        inline void SetSkybox(const Ref<Rendering::TextureCubemap> skybox) { m_skybox = skybox; }
    };

    class World {
    private:
        WorldEnvironment m_environment;

        Vector<Entity *> m_root_entities;
        Vector<MeshRendererComponent *> m_mesh_renderers;
        Vector<LightComponent *> m_lights;
    public:
        inline WorldEnvironment &GetEnvironment() { return m_environment; }

        inline const Vector<Entity *> &GetRootEntites() const { return m_root_entities; }
        inline const Vector<MeshRendererComponent *> GetMeshRenderers() const { return m_mesh_renderers; }
        inline const Vector<LightComponent *> GetLights() const { return m_lights; }
    private:
        World() = default;
        ~World() = default;

        void AddRootEntity(Entity *entity);
        void RemoveRootEntity(Entity *entity);

        void AddMeshRenderer(MeshRendererComponent *mesh_renderer);
        void RemoveMeshRenderer(MeshRendererComponent *mesh_renderer);

        void AddLight(LightComponent *light);
        void RemoveLight(LightComponent *light);

        friend class Entity;
        friend class WorldManager;
        friend class TransformComponent;
        friend class MeshRendererComponent;
        friend class LightComponent;
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