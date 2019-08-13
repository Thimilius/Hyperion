#pragma once

#include "hyperion/common.hpp"
#include "hyperion/rendering/shader.hpp"
#include "hyperion/rendering/vertex_array.hpp"
#include "hyperion/math/mat4.hpp"
#include "hyperion/entity/camera.hpp"

namespace Hyperion::Rendering {

    class CRenderer {
    public:
        static void Begin(const CCamera &camera);
        static void End();

        static void Submit(const TRef<CShader> &shader, const TRef<CVertexArray> &vertex_array, const Math::SMat4 &transform);
    private:
        struct SState {
            struct {
                Math::SMat4 view;
                Math::SMat4 projection;

                Math::SMat4 view_projection;
            } transform;
        };

        inline static SState *s_state = new SState();
    };

}