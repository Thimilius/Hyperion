#pragma once

#include "hyperion/common.hpp"
#include "hyperion/rendering/shader.hpp"
#include "hyperion/rendering/vertex_format.hpp"
#include "hyperion/rendering/camera.hpp"
#include "hyperion/math/mat4.hpp"

namespace Hyperion::Rendering {

    class CImmediateRenderer {
    private:
        struct SState {
            struct {
                Math::SMat4 view;
                Math::SMat4 projection;

                Math::SMat4 view_projection;
            } transform;
        };

        inline static SState s_immediate_state;
        inline static u32 s_immediate_vertex_offset;
        inline static TRef<CShader> s_immediate_shader;
        inline static SVertexPNCU s_immediate_buffer[2000];
        inline static TRef<CVertexArray> s_immediate_vertex_array;
        inline static TRef<CVertexBuffer> s_immediate_vertex_buffer;
    public:
        static void Begin(const TRef<CCamera> &camera);
        static void DrawCube(Math::SVec3 center, Math::SVec3 size, Math::SVec4 color);
        static void End();
    private:
        CImmediateRenderer() = delete;
        ~CImmediateRenderer() = delete;

        static void AddVertex(Math::SVec3 position, Math::SVec3 normal, Math::SVec2 uv, Math::SVec4 color);
    };

}