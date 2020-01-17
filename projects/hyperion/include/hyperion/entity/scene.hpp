#pragma once

#include "hyperion/entity/entity.hpp"

namespace Hyperion {

    class Scene {
    private:
        Vector<Entity *> m_root_entities;
    public:
        const Vector<Entity *> &GetRootEntites() const { return m_root_entities; }
    private:
        Scene() = default;
        ~Scene() = default;

        void AddRootEntity(Entity *entity);
        void RemoveRootEntity(Entity *entity);

        friend class Entity;
        friend class SceneManager;
        friend class TransformComponent;
    };

    class SceneManager {
    private:
        // FIXME: This is probably not the right way to force a default active scene
        inline static Scene *s_active_scene = new Scene();

        inline static Vector<Scene *> s_scenes = { s_active_scene };
    public:
        inline static Scene *GetActiveScene() { return s_active_scene; }
        inline static void SetActiveScene(Scene *scene) { s_active_scene = scene; }
    private:
        SceneManager() = delete;
        ~SceneManager() = delete;
    };

}