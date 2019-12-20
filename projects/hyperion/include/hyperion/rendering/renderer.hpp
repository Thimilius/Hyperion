#pragma once

#include "hyperion/common.hpp"
#include "hyperion/rendering/shader.hpp"
#include "hyperion/rendering/mesh.hpp"
#include "hyperion/rendering/camera.hpp"
#include "hyperion/math/mat4.hpp"

namespace Hyperion::Rendering {

    class Renderer {
    private:
        struct State {
            struct {
                Mat4 view;
                Mat4 projection;

                Mat4 view_projection;
            } transform;
        };

        inline static State s_state;
    public:
        static void Begin(const Ref<Camera> &camera);
        static void Submit(const Ref<Mesh> &mesh, const Ref<Shader> &shader, const Mat4 &transform);
        static void End();
    private:
        Renderer() = delete;
        ~Renderer() = delete;
    };

}