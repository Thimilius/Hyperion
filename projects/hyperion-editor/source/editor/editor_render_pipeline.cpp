#include "hyperion/editor/editor_render_pipeline.hpp"

#include <hyperion/core/math/math.hpp>
#include <hyperion/core/image.hpp>
#include <hyperion/core/app/time.hpp>
#include <hyperion/core/app/display.hpp>
#include <hyperion/core/io/image_loader.hpp>
#include <hyperion/rendering/render_driver.hpp>

using namespace Hyperion::Rendering;

namespace Hyperion::Editor {

    void EditorRenderPipeline::Initialize(Rendering::IRenderDriver *render_driver) {
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

        RasterizerState rasterizer_state;
        rasterizer_state.blending_enabled = true;
        render_driver->SetRasterizerState(rasterizer_state);

        Vector<RenderTextureAttachment> attachments = {
            { RenderTextureFormat::RGBA32, TextureParameters() },
            { RenderTextureFormat::Depth24Stencil8, TextureParameters() },
        };
        m_render_texture = RenderTexture::Create(Display::GetWidth(), Display::GetHeight(), attachments);
    }

    void EditorRenderPipeline::Render(Rendering::IRenderDriver *render_driver, const Rendering::RenderPipelineContext &context) {
        m_render_texture->Resize(Display::GetWidth(), Display::GetHeight());

        render_driver->SetCameraData(context.GetCameraData());

        render_driver->SetRenderTexture(m_render_texture->GetResourceId());
        Viewport viewport = { 0, 0, Display::GetWidth(), Display::GetHeight() };
        render_driver->SetViewport(viewport);

        Color color = Color::Cyan();
        float32 value = Math::Sin(Time::GetTime() * 2.0f) / 2.0f + 0.5f;
        color *= value;
        render_driver->Clear(ClearFlags::Color | ClearFlags::Depth | ClearFlags::Stencil, color);

        render_driver->DrawMesh(m_mesh->GetResourceId(), Mat4::Identity(), m_material->GetResourceId(), 0);

        render_driver->BlitRenderTexture(0, Display::GetWidth(), Display::GetHeight(), m_render_texture->GetResourceId(), m_render_texture->GetWidth(), m_render_texture->GetHeight());
    }

    void EditorRenderPipeline::Shutdown(Rendering::IRenderDriver *render_driver) {

    }

}