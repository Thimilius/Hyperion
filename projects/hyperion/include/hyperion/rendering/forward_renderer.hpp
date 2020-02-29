#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/math/mat4.hpp"
#include "hyperion/rendering/shader.hpp"
#include "hyperion/rendering/material.hpp"
#include "hyperion/rendering/mesh.hpp"
#include "hyperion/rendering/camera.hpp"
#include "hyperion/rendering/texture_cubemap.hpp"
#include "hyperion/entity/world.hpp"

namespace Hyperion::Rendering {

    class ForwardRenderer {
    private:
        struct State {
            struct {
                Mat4 view;
                Mat4 projection;

                Mat4 view_projection;
            } transform;

            Ref<Camera> camera;
        };

        struct Skybox {
            Ref<Shader> shader;
            Ref<Mesh> mesh;
        };

        inline static State s_state;
        inline static Skybox s_skybox;
    public:
        static void Begin(const Ref<Camera> &camera);
        static void DrawSkybox(const Ref<TextureCubemap> &skybox);
        static void DrawWorld(World *world);
        static void DrawMesh(const Ref<Mesh> &mesh, const Ref<Material> &material, const Mat4 &transform);
        static void DrawMesh(const Ref<Mesh> &mesh, const Ref<Material> &material, const Mat4 &transform, const Mat4 &inverse_transform);
        static void End();
    private:
        ForwardRenderer() = delete;
        ~ForwardRenderer() = delete;

        static void Init();

        static void PrepareShader(const Ref<Shader> &shader, const Mat4 &transform, const Mat4 &inverse_transform);
        static void DrawCall(const Ref<Mesh> &mesh);

        friend class Hyperion::Engine;
    };

}