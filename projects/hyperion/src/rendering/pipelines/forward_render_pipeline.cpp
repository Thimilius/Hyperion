#include "hyppch.hpp"

#include "hyperion/rendering/pipelines/forward_render_pipeline.hpp"

#include "hyperion/assets/font.hpp"
#include "hyperion/assets/material.hpp"
#include "hyperion/assets/mesh.hpp"
#include "hyperion/assets/shader.hpp"
#include "hyperion/assets/texture.hpp"
#include "hyperion/core/app/time.hpp"
#include "hyperion/rendering/render_engine.hpp"

namespace Hyperion::Rendering {

    Shader *g_shader;
    Material *g_material;
    Mesh *g_mesh;
    Texture *g_texture;
    Font *g_font;

    void ForwardRenderPipeline::Init() {
        Map<ShaderStageFlags, String> sources = {
            { ShaderStageFlags::Vertex, R"(
                #version 410 core
                
                layout(location = 0) in vec3 a_position;
                layout(location = 1) in vec3 a_normal;
                layout(location = 2) in vec2 a_texture0;

                void main() {
	                gl_Position = vec4(a_position, 1.0);
                }
            )" },
            { ShaderStageFlags::Fragment, R"(
                #version 410 core

                out vec4 o_color;

                uniform vec4 u_color;

                void main() {
	                o_color = u_color;
                }
            )" }
        };
        g_shader = Shader::Create(sources);

        g_material = Material::Create(g_shader);

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

        g_font = FontLoader::LoadFont("data/fonts/consola.ttf", 16, FontCharacterSet::ASCII);

        RenderEngine::GetRenderDriver()->SetRasterizerState(RasterizerState());
    }

    void ForwardRenderPipeline::Render(const RenderContext &context) {
        Viewport viewport = { 0, 0, static_cast<int32>(Display::GetWidth()), static_cast<int32>(Display::GetHeight()) };
        RenderEngine::GetRenderDriver()->Viewport(viewport);

        Color color = Color::Cyan();
        float32 value = Math::Sin(Time::GetTime() * 2.0f) / 2.0f + 0.5f;
        color *= value;
        RenderEngine::GetRenderDriver()->Clear(ClearFlags::Color | ClearFlags::Depth | ClearFlags::Stencil, color);

        color = Color::Green();
        value = Math::Sin(Time::GetTime() * 2.0f + Math::PI) / 2.0f + 0.5f;
        color *= value;
        g_material->SetVec4("u_color", color);
        RenderEngine::GetRenderDriver()->DrawIndexed(g_mesh->GetResourceId(), g_material->GetResourceId());
    }

}