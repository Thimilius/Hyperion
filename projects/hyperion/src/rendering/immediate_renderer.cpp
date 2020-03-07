#include "hyppch.hpp"

#include "hyperion/rendering/immediate_renderer.hpp"

#include "hyperion/rendering/render_command.hpp"
#include "hyperion/assets/asset_manager.hpp"

namespace Hyperion::Rendering {

    void ImmediateRenderer::Begin(const CameraData &camera, MeshTopology topology) {
        s_state.transform.view = camera.view_matrix;
        s_state.transform.projection = camera.projection_matrix;
        s_state.transform.view_projection = camera.view_projection_matrix;
        s_state.topology = topology;
    }

    void ImmediateRenderer::DrawText(const String &text, const Ref<Font> &font, f32 x, f32 y, f32 scale, Color color) {
        s_font_resources.shader->Bind();
        s_font_resources.shader->SetVec4("u_color", color);
        Mat4 projection = Mat4::Orthographic(0, static_cast<f32>(Display::GetWidth()), 0, static_cast<f32>(Display::GetHeight()), -1.0f, 1.0f);
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

            RenderCommand::Draw(MeshTopology::Triangles, 6, 0);

            x += glyph.advance * scale;
        }
    }

    void ImmediateRenderer::DrawCube(Vec3 center, Vec3 size, Color color) {
        Vec3 half_size = size * 0.5f;

        // Forward
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), color);

        // Right
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z), color);

        // Back
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), color);

        // Left
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z), color);

        // Up
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z), color);

        // Down
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), color);
        AddVertex(Vec3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z), color);
    }

    void ImmediateRenderer::DrawLine(Vec3 a, Vec3 b, Color color) {
        AddVertex(a, color);
        AddVertex(b, color);
    }

    void ImmediateRenderer::DrawWire(MeshTopology topology, const Ref<VertexArray> &vertex_array, u32 vertex_count) {
        s_immediate_resources.shader->Bind();
        s_immediate_resources.shader->SetMat4("u_transform.view", s_state.transform.view);
        s_immediate_resources.shader->SetMat4("u_transform.projection", s_state.transform.projection);

        vertex_array->Bind();

        RenderCommand::Draw(topology, vertex_count, 0);
    }

    void ImmediateRenderer::DrawWireCube(Vec3 center, Vec3 size, Color color) {
        Vec3 half_size = size * 0.5f;

        Vec3 min = center + half_size;
        Vec3 max = center - half_size;

        AddVertex(Vec3(max.x, max.y, min.z), color);
        AddVertex(Vec3(max.x, min.y, min.z), color);
        AddVertex(Vec3(max.x, min.y, min.z), color);
        AddVertex(Vec3(min.x, min.y, min.z), color);
        AddVertex(Vec3(min.x, min.y, min.z), color);
        AddVertex(Vec3(min.x, max.y, min.z), color);
        AddVertex(Vec3(min.x, max.y, min.z), color);
        AddVertex(Vec3(max.x, max.y, min.z), color);

        AddVertex(Vec3(max.x, max.y, min.z), color);
        AddVertex(Vec3(max.x, max.y, max.z), color);
        AddVertex(Vec3(max.x, min.y, min.z), color);
        AddVertex(Vec3(max.x, min.y, max.z), color);
        AddVertex(Vec3(min.x, min.y, min.z), color);
        AddVertex(Vec3(min.x, min.y, max.z), color);
        AddVertex(Vec3(min.x, max.y, min.z), color);
        AddVertex(Vec3(min.x, max.y, max.z), color);

        AddVertex(Vec3(max.x, max.y, max.z), color);
        AddVertex(Vec3(max.x, min.y, max.z), color);
        AddVertex(Vec3(max.x, min.y, max.z), color);
        AddVertex(Vec3(min.x, min.y, max.z), color);
        AddVertex(Vec3(min.x, min.y, max.z), color);
        AddVertex(Vec3(min.x, max.y, max.z), color);
        AddVertex(Vec3(min.x, max.y, max.z), color);
        AddVertex(Vec3(max.x, max.y, max.z), color);
    }

    void ImmediateRenderer::Init() {
        s_immediate_resources.shader = AssetManager::GetShader("standard_immediate");
        s_immediate_resources.vertex_buffer = VertexBuffer::Create(nullptr, sizeof(s_immediate_resources.data_buffer), BufferUsage::DynamicDraw);
        s_immediate_resources.vertex_buffer->SetLayout(VertexImmediate::GetBufferLayout());
        s_immediate_resources.vertex_array = VertexArray::Create();
        s_immediate_resources.vertex_array->AddVertexBuffer(s_immediate_resources.vertex_buffer);

        s_font_resources.shader = AssetManager::GetShader("standard_font");
        s_font_resources.vertex_buffer = VertexBuffer::Create(nullptr, 6 * 4 * sizeof(f32), BufferUsage::DynamicDraw);
        s_font_resources.vertex_buffer->SetLayout(VertexFont::GetBufferLayout());
        s_font_resources.vertex_array = VertexArray::Create();
        s_font_resources.vertex_array->AddVertexBuffer(s_font_resources.vertex_buffer);
    }

    void ImmediateRenderer::AddVertex(Vec3 position, Color color) {
        u32 vertex_offset = s_state.vertex_offset;
        if (vertex_offset >= s_immediate_resources.DATA_BUFFER_SIZE) {
            Flush(s_state.topology);
            s_state.vertex_offset = 0;
            vertex_offset = 0;
        }

        s_immediate_resources.data_buffer[vertex_offset].position = position;
        s_immediate_resources.data_buffer[vertex_offset].color = color;

        s_state.vertex_offset++;
    }

    void ImmediateRenderer::End() {
        Flush(s_state.topology);
    }

    void ImmediateRenderer::Flush(MeshTopology topology) {
        // We do not need to draw anything if no verticies were added
        if (s_state.vertex_offset == 0) {
            return;
        }

        s_immediate_resources.vertex_buffer->SetData(0, s_state.vertex_offset * sizeof(VertexImmediate), (u8*)s_immediate_resources.data_buffer);

        DrawWire(topology, s_immediate_resources.vertex_array, s_state.vertex_offset);

        s_state.vertex_offset = 0;
    }

}