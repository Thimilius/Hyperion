#pragma once

#include "hyperion/common.hpp"
#include "hyperion/core/math/mat4.hpp"
#include "hyperion/rendering/shader.hpp"
#include "hyperion/rendering/camera.hpp"
#include "hyperion/rendering/vertex_format.hpp"

namespace Hyperion::Rendering {

    class ImmediateRenderer {
    private:
        struct State {
            struct {
                Mat4 view;
                Mat4 projection;

                Mat4 view_projection;
            } transform;

            u32 vertex_offset;
        };

        inline static State s_state;
        inline static Ref<Shader> s_immediate_shader;
        inline static VertexImmediate s_data_buffer[2000];
        inline static Ref<VertexArray> s_vertex_array;
        inline static Ref<VertexBuffer> s_vertex_buffer;
    public:
        static void Begin(const Ref<Camera> &camera);
        static void Draw(PrimitiveType primitive_type, const Ref<VertexArray> &vertex_array, u32 vertex_count);
        static void DrawCube(Vec3 center, Vec3 size, Color color);
        static void DrawLine(Vec3 a, Vec3 b, Color color);
        static void End();
    private:
        ImmediateRenderer() = delete;
        ~ImmediateRenderer() = delete;

        static void Init();

        static void AddVertex(Vec3 position, Color color);
        static void Flush(PrimitiveType type);

        friend class Hyperion::Engine;
    };

}