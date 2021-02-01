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
                layout(location = 4) in vec2 a_texture0;

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

        InitializeGrid();
    }

    void EditorRenderPipeline::Render(Rendering::IRenderDriver *render_driver, const Rendering::RenderPipelineContext &context) {
        render_driver->SetCameraData(context.GetCameraData());

        Viewport viewport = { 0, 0, Display::GetWidth(), Display::GetHeight() };
        render_driver->SetViewport(viewport);
        m_render_texture->Resize(Display::GetWidth(), Display::GetHeight());
        render_driver->SetRenderTexture(m_render_texture->GetResourceId());

        render_driver->Clear(ClearFlags::Color | ClearFlags::Depth | ClearFlags::Stencil, Color::Black());
        render_driver->DrawMesh(m_mesh->GetResourceId(), Mat4::Identity(), m_material->GetResourceId(), 0);
        render_driver->DrawMesh(m_grid_mesh->GetResourceId(), Mat4::Identity(), m_grid_material->GetResourceId(), 0);

        render_driver->BlitRenderTexture(0, Display::GetWidth(), Display::GetHeight(), m_render_texture->GetResourceId(), m_render_texture->GetWidth(), m_render_texture->GetHeight());
    }

    void EditorRenderPipeline::Shutdown(Rendering::IRenderDriver *render_driver) {

    }

    void EditorRenderPipeline::InitializeGrid() {
        Map<ShaderStageFlags, String> sources = {
            { ShaderStageFlags::Vertex, R"(
                #version 410 core
                
                layout(location = 0) in vec3 a_position;
                layout(location = 3) in vec4 a_color;

                out V2F {
	                vec4 color;
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
                    o_v2f.color = a_color;

	                gl_Position = obj_to_clip_space(a_position);
                }
            )" },
            { ShaderStageFlags::Fragment, R"(
                #version 410 core

                layout(location = 0) out vec4 o_color;

                in V2F {
	                vec4 color;
                } i_v2f;

                void main() {
	                o_color = i_v2f.color;
                }
            )" }
        };
        Shader *grid_shader = Shader::Create(sources);
        m_grid_material = Material::Create(grid_shader);

        const Color default_grid_color = Color(0.25f, 0.25f, 0.25f, 0.25f);
        const Color special_grid_color = Color(0.75f, 0.75f, 0.75f, 0.75f);
        const int32 grid_size = 100;

        int32 half_grid_size = grid_size / 2;
        float32 to_point = static_cast<float32>(half_grid_size);

        uint32 grid_vertex_count = ((grid_size) * 4) + 6;
        MeshData mesh_data;
        mesh_data.positions.resize(grid_vertex_count);
        mesh_data.colors.resize(grid_vertex_count);
        mesh_data.indices.resize(grid_vertex_count);

        uint32 index = 0;
        for (int32 x = -half_grid_size; x <= half_grid_size; x++) {
            if (x == 0) continue; // Skip center line

            float32 from_point = static_cast<float32>(x);
            Color color = (x % 10) == 0 ? special_grid_color : default_grid_color;
            mesh_data.positions[index] = Vec3(from_point, 0, to_point);
            mesh_data.indices[index] = index;
            mesh_data.colors[index] = color;
            index++;
            mesh_data.positions[index] = Vec3(from_point, 0, -to_point);
            mesh_data.indices[index] = index;
            mesh_data.colors[index] = color;
            index++;
        }
        for (int32 z = -half_grid_size; z <= half_grid_size; z++) {
            if (z == 0) continue; // Skip center line

            float32 from_point = static_cast<float32>(z);
            Color color = (z % 10) == 0 ? special_grid_color : default_grid_color;
            mesh_data.positions[index] = Vec3(to_point, 0, from_point);
            mesh_data.indices[index] = index;
            mesh_data.colors[index] = color;
            index++;
            mesh_data.positions[index] = Vec3(-to_point, 0, from_point);
            mesh_data.indices[index] = index;
            mesh_data.colors[index] = color;
            index++;
        }

        const float32 axis_length = 1000.0f;
        {
            mesh_data.positions[index] = Vec3(-axis_length, 0, 0);
            mesh_data.indices[index] = index;
            mesh_data.colors[index] = Color::Red();
            index++;
            mesh_data.positions[index] = Vec3(axis_length, 0, 0);
            mesh_data.indices[index] = index;
            mesh_data.colors[index] = Color::Red();
            index++;
            mesh_data.positions[index] = Vec3(0, -axis_length, 0);
            mesh_data.indices[index] = index;
            mesh_data.colors[index] = Color::Green();
            index++;
            mesh_data.positions[index] = Vec3(0, axis_length, 0);
            mesh_data.indices[index] = index;
            mesh_data.colors[index] = Color::Green();
            index++;
            mesh_data.positions[index] = Vec3(0, 0, -axis_length);
            mesh_data.indices[index] = index;
            mesh_data.colors[index] = Color::Blue();
            index++;
            mesh_data.positions[index] = Vec3(0, 0, axis_length);
            mesh_data.indices[index] = index;
            mesh_data.colors[index] = Color::Blue();
            index++;
        }

        Vector<SubMesh> sub_meshes = { { MeshTopology::Lines, grid_vertex_count, 0, 0 } };
        m_grid_mesh = Mesh::Create(mesh_data, sub_meshes);
    }

}