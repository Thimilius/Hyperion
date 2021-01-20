#include "hyppch.hpp"

#include "hyperion/rendering/pipelines/forward_render_pipeline.hpp"

#include "hyperion/assets/mesh.hpp"
#include "hyperion/assets/shader.hpp"
#include "hyperion/assets/texture.hpp"
#include "hyperion/core/app/time.hpp"
#include "hyperion/rendering/render_engine.hpp"

namespace Hyperion::Rendering {

    Shader *g_shader;
    Mesh *g_mesh;
    Texture *g_texture;

    void ForwardRenderPipeline::Init() {
        Map<ShaderStageFlags, String> sources = {
            { ShaderStageFlags::Vertex, R"(
                #version 410 core
                
                layout(location = 0) in vec3 a_position;

                void main() {
	                gl_Position = vec4(a_position, 1.0);
                }
            )" },
            { ShaderStageFlags::Fragment, R"(
                #version 410 core

                out vec4 o_color;

                void main() {
	                o_color = vec4(0.9, 0.5, 0, 1);
                }
            )" }
        };
        g_shader = Shader::Create(sources);

        MeshData mesh_data;
        mesh_data.positions = {
            Vec3(-0.5f,  0.5f, 0.0f),
            Vec3( 0.5f,  0.5f, 0.0f),
            Vec3( 0.5f, -0.5f, 0.0f),
            Vec3(-0.5f, -0.5f, 0.0f),
        };
        mesh_data.normals.resize(4);
        mesh_data.texture0.resize(4);
        mesh_data.indices = {
            0, 1, 2,
            0, 2, 3
        };
        Vector<SubMesh> sub_meshes = {
            { MeshTopology::Triangles, 3, 0, 0 },
            { MeshTopology::Triangles, 3, 3, 0 }
        };

        g_mesh = Mesh::Create(mesh_data, sub_meshes);

        uint32 width = 1024;
        uint32 height = 1024;
        Vector<uint8> pixels(width * height * 32);
        uint8 *data = pixels.data();
        for (uint32 x = 0; x < width; x++) {
            for (uint32 y = 0; y < height; y++) {
                uint32 *pixel = reinterpret_cast<uint32 *>(&data[(x + (y * width)) * 4]);
                // NOTE: The pixel format is: 0xAABBGGRR
                *pixel = 0xFF00FFFF;
            }
        }

        g_texture = Texture2D::Create(width, height, TextureFormat::RGBA32, TextureParameters(), pixels);
        //Object::Destroy(g_texture);

        RenderEngine::GetRenderDriver()->SetRasterizerState(RasterizerState());
    }

    void ForwardRenderPipeline::Render() {
        Viewport viewport = { 0, 0, static_cast<int32>(Display::GetWidth()), static_cast<int32>(Display::GetHeight()) };
        RenderEngine::GetRenderDriver()->Viewport(viewport);

        Color color = Color::Cyan();
        float32 value = Math::Sin(Time::GetTime() * 2.0f) / 2.0f + 0.5f;
        color *= value;
        RenderEngine::GetRenderDriver()->Clear(ClearFlags::Color | ClearFlags::Depth | ClearFlags::Stencil, color);

        RenderEngine::GetRenderDriver()->DrawIndexed(g_shader->GetResourceId(), g_mesh->GetResourceId());
    }

}