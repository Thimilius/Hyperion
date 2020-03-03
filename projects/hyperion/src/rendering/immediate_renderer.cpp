#include "hyppch.hpp"

#include "hyperion/rendering/immediate_renderer.hpp"

#include "hyperion/assets/asset_library.hpp"

namespace Hyperion::Rendering {

    void ImmediateRenderer::Begin(const CameraData &camera) {
        s_state.transform.view = camera.view_matrix;
        s_state.transform.projection = camera.projection_matrix;
        s_state.transform.view_projection = camera.view_projection_matrix;
    }

    void ImmediateRenderer::Draw(MeshTopology topology, const Ref<VertexArray> &vertex_array, u32 vertex_count) {
        s_immediate_resources.shader->Bind();
        s_immediate_resources.shader->SetMat4("u_transform.view", s_state.transform.view);
        s_immediate_resources.shader->SetMat4("u_transform.projection", s_state.transform.projection);

        vertex_array->Bind();

        RenderEngine::Draw(topology, vertex_count, 0);
    }

    void ImmediateRenderer::DrawCube(Vec3 center, Vec3 size, Color m_color) {
        Vec3 half_size = size * 0.5f;

        // Forward
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z), m_color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), m_color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z), m_color);
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z), m_color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z), m_color);
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), m_color);

        // Right
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), m_color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z), m_color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), m_color);
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), m_color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), m_color);
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z), m_color);

        // Back
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z), m_color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), m_color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z), m_color);
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z), m_color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z), m_color);
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), m_color);

        // Left
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), m_color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z), m_color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), m_color);
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), m_color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), m_color);
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z), m_color);

        // Up
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), m_color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z), m_color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), m_color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), m_color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), m_color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z), m_color);

        // Down
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), m_color);
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z), m_color);
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), m_color);
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), m_color);
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), m_color);
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z), m_color);

        Flush(MeshTopology::Triangles);
    }

    void ImmediateRenderer::DrawLine(Vec3 a, Vec3 b, Color m_color) {
        AddVertex(a, m_color);
        AddVertex(b, m_color);

        Flush(MeshTopology::Lines);
    }

    void ImmediateRenderer::DrawText(const String &text, const Ref<Font> &font, f32 x, f32 y, f32 scale, Color color) {
        s_font_resources.shader->Bind();
        s_font_resources.shader->SetVec4("u_color", color);
        Window *window = Application::GetInstance()->GetWindow();
        Mat4 projection = Mat4::Orthographic(0, (f32)window->GetWidth(), 0, (f32)window->GetHeight(), -1.0f, 1.0f);
        s_font_resources.shader->SetMat4("u_transform.projection", projection);

        s_font_resources.vertex_array->Bind();

        for (char c : text) {
            FontGlyph glyph = font->GetGlyph(c);

            f32 xpos = x + glyph.bearing.x * scale;
            f32 ypos = y - (glyph.size.y - glyph.bearing.y) * scale;

            f32 w = glyph.size.x * scale;
            f32 h = glyph.size.y * scale;

            // NOTE: The uvs are flipped horizontaly because the textures are loaded in flipped
            f32 vertices[6][4] = {
                { xpos + w, ypos + h,   1.0, 0.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos,     ypos,       0.0, 1.0 },

                { xpos + w, ypos + h,   1.0, 0.0 },
                { xpos,     ypos,       0.0, 1.0 },
                { xpos,     ypos + h,   0.0, 0.0 },
            };

            glyph.texture->Bind();
            s_font_resources.vertex_buffer->SetData(0, sizeof(vertices), (u8 *)vertices);

            RenderEngine::Draw(MeshTopology::Triangles, 6, 0);

            x += glyph.advance * scale;
        }
    }

    void ImmediateRenderer::Init() {
        s_immediate_resources.shader = AssetLibrary::GetShader("standard_immediate");
        s_immediate_resources.vertex_buffer = VertexBuffer::Create(nullptr, sizeof(s_immediate_resources.data_buffer), BufferUsage::DynamicDraw);
        s_immediate_resources.vertex_buffer->SetLayout(VertexImmediate::GetBufferLayout());
        s_immediate_resources.vertex_array = VertexArray::Create();
        s_immediate_resources.vertex_array->AddVertexBuffer(s_immediate_resources.vertex_buffer);

        s_font_resources.shader = AssetLibrary::GetShader("standard_font");
        s_font_resources.vertex_buffer = VertexBuffer::Create(nullptr, 6 * 4 * sizeof(f32), BufferUsage::DynamicDraw);
        s_font_resources.vertex_buffer->SetLayout(VertexFont::GetBufferLayout());
        s_font_resources.vertex_array = VertexArray::Create();
        s_font_resources.vertex_array->AddVertexBuffer(s_font_resources.vertex_buffer);
    }

    void ImmediateRenderer::AddVertex(Vec3 position, Color color) {
        u32 vertex_offset = s_state.vertex_offset;
        HYP_ASSERT_MESSAGE(vertex_offset < 2000, "Immediate vertex buffer is full!");

        s_immediate_resources.data_buffer[vertex_offset].position = position;
        s_immediate_resources.data_buffer[vertex_offset].color = color;

        s_state.vertex_offset++;
    }

    void ImmediateRenderer::End() {
        
    }

    void ImmediateRenderer::Flush(MeshTopology topology) {
        // We do not need to draw anything if no verticies were added
        if (s_state.vertex_offset == 0) {
            return;
        }

        s_immediate_resources.shader->Bind();
        s_immediate_resources.shader->SetMat4("u_transform.view", s_state.transform.view);
        s_immediate_resources.shader->SetMat4("u_transform.projection", s_state.transform.projection);

        s_immediate_resources.vertex_buffer->SetData(0, s_state.vertex_offset * sizeof(VertexImmediate), (u8*)s_immediate_resources.data_buffer);
        s_immediate_resources.vertex_array->Bind();

        RenderEngine::Draw(topology, s_state.vertex_offset, 0);

        s_state.vertex_offset = 0;
    }

}