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

            u32 vertex_offset;
        };

        struct SVertexImmediate {
            Math::SVec3 position;
            Math::SVec3 normal;
            Math::SVec4 color;
            Math::SVec2 uv;

            static CBufferLayout GetBufferLayout() {
                return CBufferLayout({
                    SBufferElement("a_position", EShaderDataType::Float3),
                    SBufferElement("a_normal", EShaderDataType::Float3),
                    SBufferElement("a_color", EShaderDataType::Float4),
                    SBufferElement("a_uv", EShaderDataType::Float2)
                });
            }
        };

        inline static SState s_state;
        inline static TRef<CShader> s_immediate_shader;
        inline static SVertexImmediate s_data_buffer[2000];
        inline static TRef<CVertexArray> s_vertex_array;
        inline static TRef<CVertexBuffer> s_vertex_buffer;
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