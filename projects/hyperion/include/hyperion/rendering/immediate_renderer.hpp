#pragma once

#include "hyperion/rendering/shader.hpp"
#include "hyperion/rendering/font.hpp"
#include "hyperion/rendering/mesh.hpp"
#include "hyperion/rendering/camera_data.hpp"
#include "hyperion/rendering/vertex_format.hpp"
#include "hyperion/rendering/vertex_array.hpp"

namespace Hyperion::Rendering {
    class RenderEngine;
}

namespace Hyperion::Rendering {

    class ImmediateRenderer {
    public:
        static void SetCameraData(const CameraData &camera);

        static void DrawText(const String &text, const Ref<Font> &font, f32 x, f32 y, f32 scale, Color color);
        static void DrawVertexArray(MeshTopology topology, const Ref<VertexArray> &vertex_array, u32 vertex_count);

        static void Begin(MeshTopology topology = MeshTopology::Triangles);
        static void DrawCube(Vec3 center, Vec3 size, Color color);
        static void DrawLine(Vec3 a, Vec3 b, Color color);
        static void DrawWireCube(Vec3 center, Vec3 size, Color color);
        static void End();
    private:
        ImmediateRenderer() = delete;
        ~ImmediateRenderer() = delete;

        static void Init();

        static void AddVertex(Vec3 position, Color color);
        static void Flush(MeshTopology topology);
    private:
        struct State {
            struct Transform {
                Mat4 view;
                Mat4 projection;

                Mat4 view_projection;
            } transform;

            MeshTopology topology;
            u32 vertex_offset;
        };

        struct ImmediateResources {
            inline static const u32 DATA_BUFFER_SIZE = 2048;

            Ref<Shader> shader;
            Ref<VertexArray> vertex_array;
            Ref<VertexBuffer> vertex_buffer;

            VertexImmediate data_buffer[DATA_BUFFER_SIZE];
        };
        
        struct FontResources {
            Ref<Shader> shader;
            Ref<VertexArray> vertex_array;
            Ref<VertexBuffer> vertex_buffer;
        };
        
        inline static State s_state;
        inline static ImmediateResources s_immediate_resources;
        inline static FontResources s_font_resources;
    private:
        friend class Hyperion::Rendering::RenderEngine;
    };

}