#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/color.hpp"

namespace Hyperion {

    namespace Rendering {
        class TextureCubemap;
    }
    class Entity;
    class MeshRenderer;
    class Light;
    class Camera;

    struct WorldEnvironment {
        struct Background {
            enum class Mode {
                Color,
                Skybox
            };

            Mode mode = Mode::Color;
            Color color = Color::Black();
            Ref<Rendering::TextureCubemap> skybox;
        } background;

        struct AmbientLight {
            f32 intensity = 0.2f;
            Color color = Color::White();
        } ambient_light;
    };

    class World {
    private:
        WorldEnvironment m_environment;

        Vector<Entity *> m_root_entities;

        Vector<Camera *> m_cameras;
        Vector<MeshRenderer *> m_mesh_renderers;
        Vector<Light *> m_lights;
    public:
        inline WorldEnvironment &GetEnvironment() { return m_environment; }

        inline const Vector<Entity *> &GetRootEntites() const { return m_root_entities; }

        inline const Vector<Camera *> &GetCameras() const { return m_cameras; }
        inline const Vector<MeshRenderer *> &GetMeshRenderers() const { return m_mesh_renderers; }
        inline const Vector<Light *> &GetLights() const { return m_lights; }
    private:
        World() = default;
        ~World();

        void AddRootEntity(Entity *entity);
        void RemoveRootEntity(Entity *entity);

        void AddCamera(Camera *camera);
        void RemoveCamera(Camera *camera);
        void AddMeshRenderer(MeshRenderer *mesh_renderer);
        void RemoveMeshRenderer(MeshRenderer *mesh_renderer);
        void AddLight(Light *light);
        void RemoveLight(Light *light);

        friend class Entity;
        friend class WorldManager;
        friend class Transform;
        friend class MeshRenderer;
        friend class Light;
        friend class Camera;
    };

}