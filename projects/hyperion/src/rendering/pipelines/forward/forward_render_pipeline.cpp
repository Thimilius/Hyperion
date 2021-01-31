#include "hyppch.hpp"

#include "hyperion/rendering/pipelines/forward/forward_render_pipeline.hpp"

#include "hyperion/core/app/time.hpp"
#include "hyperion/core/io/image_loader.hpp"
#include "hyperion/rendering/render_driver.hpp"

namespace Hyperion::Rendering {

    void ForwardRenderPipeline::Initialize(IRenderDriver *render_driver) {
        Map<ShaderStageFlags, String> sources = {
            { ShaderStageFlags::Vertex, R"(
                #version 410 core
                
                layout(location = 0) in vec3 a_position;
                layout(location = 1) in vec3 a_normal;
                layout(location = 2) in vec2 a_texture0;

                out V2F {
	                vec2 texture0;
                } o_v2f;

                void main() {
                    o_v2f.texture0 = a_texture0;

	                gl_Position = vec4(a_position, 1.0);
                }
            )" },
            { ShaderStageFlags::Fragment, R"(
                #version 410 core

                layout(location = 0) out vec4 o_color;

                in V2F {
	                vec2 texture0;
                } i_v2f;

                uniform vec4 u_color;
                uniform sampler2D u_texture;

                void main() {
                    vec4 texture_color = vec4(1.0, 1.0, 1.0, texture(u_texture, i_v2f.texture0).r);
	                o_color = u_color * texture_color;
                }
            )" }
        };
        m_shader = Shader::Create(sources);
        m_material = Material::Create(m_shader);

        m_font = FontLoader::LoadFont("data/fonts/consola.ttf", 48, FontCharacterSet::ASCII);
        m_material->SetTexture("u_texture", m_font->GetTextureAtlas());
        
        const FontGlyph &glyph = m_font->GetGlyph('#');
        float32 width = m_font->GetTextWidth("#", 1.0f) / Display::GetWidth();
        float32 height = glyph.bearing.y / Display::GetHeight();
        MeshData mesh_data;
        mesh_data.positions = {
            Vec3(-width,  height, 0.0f),
            Vec3( width,  height, 0.0f),
            Vec3( width, -height, 0.0f),
            Vec3(-width, -height, 0.0f),
        };
        mesh_data.normals.resize(4);
        mesh_data.texture0 = Vector<Vec2>(glyph.uv, glyph.uv + 4);
        mesh_data.indices = {
            0, 1, 2,
            0, 2, 3,
        };
        Vector<SubMesh> sub_meshes = {
            { MeshTopology::Triangles, 6, 0, 0 },
        };
        m_mesh = Mesh::Create(mesh_data, sub_meshes);

        m_command_buffer = render_driver->CreateCommandBuffer();
        RasterizerState rasterizer_state;
        rasterizer_state.blending_enabled = true;
        m_command_buffer->SetRasterizerState(rasterizer_state);

        Vector<RenderTextureAttachment> attachments = { 
            { RenderTextureFormat::RGBA32, TextureParameters() },
            { RenderTextureFormat::Depth24Stencil8, TextureParameters() },
        };
        m_render_texture = RenderTexture::Create(Display::GetWidth(), Display::GetHeight(), attachments);

        render_driver->ExecuteCommandBuffer(m_command_buffer);
    }

    void ForwardRenderPipeline::Render(IRenderDriver *render_driver, const RenderPipelineContext &context) {
        m_command_buffer->ClearCommands();

        m_command_buffer->SetRenderTexture(m_render_texture);
        Viewport viewport = { 0, 0, Display::GetWidth(), Display::GetHeight() };
        m_command_buffer->SetViewport(viewport);

        Color color = Color::Cyan();
        float32 value = Math::Sin(Time::GetTime() * 2.0f) / 2.0f + 0.5f;
        color *= value;
        m_command_buffer->Clear(ClearFlags::Color | ClearFlags::Depth | ClearFlags::Stencil, color);

        color = Color::Green();
        value = Math::Sin(Time::GetTime() * 2.0f + Math::PI) / 2.0f + 0.5f;
        color *= value;
        m_material->SetVec4("u_color", color);
        m_command_buffer->DrawMesh(m_mesh, m_material, 0);

        m_command_buffer->Blit(nullptr, m_render_texture);

        render_driver->ExecuteCommandBuffer(m_command_buffer);
    }

    void ForwardRenderPipeline::Shutdown(IRenderDriver *render_driver) {
        render_driver->DestroyCommandBuffer(m_command_buffer);
        Object::Destroy(m_render_texture);
        Object::Destroy(m_mesh);
        Object::Destroy(m_font);
        Object::Destroy(m_material);
        Object::Destroy(m_shader);
    }

}