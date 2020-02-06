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

        struct Immediate {
            inline static const u32 DATA_BUFFER_SIZE = 2048;

            Ref<Shader> shader;
            Ref<VertexArray> vertex_array;
            Ref<VertexBuffer> vertex_buffer;

            VertexImmediate data_buffer[DATA_BUFFER_SIZE];
        };

        inline static State s_state;
        inline static Immediate s_immediate;
    public:
        static void Begin(const Ref<Camera> &camera);
        static void Draw(MeshTopology topology, const Ref<VertexArray> &vertex_array, u32 vertex_count);
        static void DrawCube(Vec3 center, Vec3 size, Color color);
        static void DrawLine(Vec3 a, Vec3 b, Color color);
        static void End();
    private:
        ImmediateRenderer() = delete;
        ~ImmediateRenderer() = delete;

        static void Init();

        static void AddVertex(Vec3 position, Color color);
        static void Flush(MeshTopology topology);

        friend class Hyperion::Engine;
    };

}