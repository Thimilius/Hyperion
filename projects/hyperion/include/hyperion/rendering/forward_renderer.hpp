#pragma once

#include "hyperion/rendering/shader.hpp"
#include "hyperion/rendering/material.hpp"
#include "hyperion/rendering/mesh.hpp"
#include "hyperion/rendering/camera_data.hpp"
#include "hyperion/rendering/texture_cubemap.hpp"

namespace Hyperion {
    class Light;
    class World;

    namespace Rendering {
        class RenderEngine;
    }
}

namespace Hyperion::Rendering {

    class ForwardRenderer {
    public:
        static void Begin(const CameraData &camera);
        static void DrawSkybox(const Ref<TextureCubemap> &skybox);
        static void DrawEntities(World *world);
        static void DrawMesh(const Ref<Mesh> &mesh, const Ref<Material> &material, const Mat4 &transform);
        static void DrawMesh(const Ref<Mesh> &mesh, const Ref<Material> &material, const Mat4 &transform, const Mat4 &inverse_transform);
        static void End();
    private:
        ForwardRenderer() = delete;
        ~ForwardRenderer() = delete;

        static void Init();

        static void PrepareMaterial(const Ref<Material> &material, const Mat4 &transform, const Mat4 &inverse_transform);
        static void DrawCall(const Ref<Mesh> &mesh);
    private:
        struct State {
            struct Transform {
                Mat4 view;
                Mat4 projection;

                Mat4 view_projection;
            } transform;

            CameraData camera;

            struct Lighting {
                inline static const u32 MAX_POINT_LIGHT_COUNT = 3;

                Vec3 ambient_color;

                Light *main_light;

                Vector<Light *> *point_lights;
                struct PointLightUniforms {
                    String intensity;
                    String color;
                    String position;
                    String range;
                } point_light_uniforms[MAX_POINT_LIGHT_COUNT];
            } lighting;
        };

        struct Skybox {
            Ref<Shader> shader;
            Ref<Mesh> mesh;
        };

        inline static State s_state;
        inline static Skybox s_skybox;
    private:
        friend class Hyperion::Rendering::RenderEngine;
    };

}