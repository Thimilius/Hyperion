#pragma once

#include "hyperion/common.hpp"
#include "hyperion/rendering/shader.hpp"
#include "hyperion/rendering/mesh.hpp"
#include "hyperion/rendering/camera.hpp"
#include "hyperion/math/mat4.hpp"

namespace Hyperion::Rendering {

    class CRenderer {
    private:
        struct SState {
            struct {
                Math::SMat4 view;
                Math::SMat4 projection;

                Math::SMat4 view_projection;
            } transform;
        };

        inline static TRef<CShader> m_immediate_shader;

        inline static SState *s_state = new SState();
    public:
        static void Begin(const TRef<CCamera> &camera);
        static void End();

        static void Submit(const TRef<CMesh> &mesh, const TRef<CShader> &shader, const Math::SMat4 &transform);

        static void ImmediateCube(Math::SVec3 center, Math::SVec3 size, Math::SVec4 color);
    private:
        CRenderer() = delete;
        ~CRenderer() = delete;
    };

}