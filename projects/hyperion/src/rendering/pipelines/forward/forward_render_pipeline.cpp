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

                uniform struct Transform {
                    mat4 model;
                    mat4 view;
                    mat4 projection;
                } u_transform;

                vec4 obj_to_clip_space(vec3 position) {
	                return u_transform.projection * u_transform.view * u_transform.model * vec4(position, 1.0);
                }

                void main() {
                    o_v2f.texture0 = a_texture0;

	                gl_Position = obj_to_clip_space(a_position);
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
                    vec4 texture_color = texture(u_texture, i_v2f.texture0);
	                o_color = u_color * texture_color;
                }
            )" }
        };
        m_shader = Shader::Create(sources);

        Image *image = ImageLoader::Load("data/textures/grass.png");
        Vector<uint8> pixels(image->GetPixels(), image->GetPixels() + (image->GetWidth() * image->GetWidth() * image->GetChannels()));
        m_texture = Texture2D::Create(image->GetWidth(), image->GetHeight(), TextureFormat::RGB24, TextureParameters(), pixels);
        delete image;

        m_material = Material::Create(m_shader);
        m_material->SetVec4("u_color", Color::White());
        m_material->SetTexture("u_texture", m_texture);

        m_mesh = MeshFactory::CreateCube(1.0f);

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

        m_command_buffer2 = render_driver->CreateCommandBuffer();
        for (float32 x = 0; x < 25; x++) {
            for (float32 z = 0; z < 25; z++) {
                m_command_buffer2->DrawMesh(m_mesh, Mat4::Translate(x * 2.0f, 0.0f, z * 2.0f), m_material, 0);
            }
        }
        m_command_buffer2->Blit(nullptr, m_render_texture);
    }

    void ForwardRenderPipeline::Render(IRenderDriver *render_driver, const RenderPipelineContext &context) {
        m_command_buffer->ClearCommands();

        m_render_texture->Resize(Display::GetWidth(), Display::GetHeight());

        m_command_buffer->SetupCameraData(context.GetCameraData());

        m_command_buffer->SetRenderTexture(m_render_texture);
        Viewport viewport = { 0, 0, Display::GetWidth(), Display::GetHeight() };
        m_command_buffer->SetViewport(viewport);
        
        Color color = Color::Cyan();
        float32 value = Math::Sin(Time::GetTime() * 2.0f) / 2.0f + 0.5f;
        color *= value;
        m_command_buffer->Clear(ClearFlags::Color | ClearFlags::Depth | ClearFlags::Stencil, color);

        //for (float32 x = 0; x < 25; x++) {
        //    for (float32 z = 0; z < 25; z++) {
        //        m_command_buffer->DrawMesh(m_mesh, Mat4::Translate(x * 2.0f, 0.0f, z * 2.0f), m_material, 0);
        //    }
        //}

        render_driver->ExecuteCommandBuffer(m_command_buffer);
        render_driver->ExecuteCommandBuffer(m_command_buffer2);
    }

    void ForwardRenderPipeline::Shutdown(IRenderDriver *render_driver) {
        render_driver->DestroyCommandBuffer(m_command_buffer);
        Object::Destroy(m_render_texture);
        Object::Destroy(m_mesh);
        Object::Destroy(m_material);
        Object::Destroy(m_shader);
    }

}