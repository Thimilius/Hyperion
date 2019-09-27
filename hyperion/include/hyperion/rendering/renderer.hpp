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

        inline static SState s_state;

        inline static SState s_immediate_state;
        inline static u32 s_immediate_vertex_offset;
        inline static TRef<CShader> s_immediate_shader;
        inline static SVertexPNU s_immediate_buffer[2000];
        inline static TRef<CVertexArray> s_immediate_vertex_array;
        inline static TRef<CVertexBuffer> s_immediate_vertex_buffer;
    public:
        static void Begin(const TRef<CCamera> &camera);
        static void Submit(const TRef<CMesh> &mesh, const TRef<CShader> &shader, const Math::SMat4 &transform);
        static void End();

        static void ImmediateBegin(const TRef<CCamera> &camera);
        static void ImmediateCube(Math::SVec3 center, Math::SVec3 size, Math::SVec4 color);
        static void ImmediateVertex(Math::SVec3 position, Math::SVec3 normal, Math::SVec2 uv, Math::SVec4 color);
        static void ImmediateEnd();
    private:
        CRenderer() = delete;
        ~CRenderer() = delete;
    };

}