#include "hyppch.hpp"

#include "hyperion/rendering/immediate_renderer.hpp"

#include "hyperion/rendering/render_command.hpp"

using namespace Hyperion::Math;

namespace Hyperion::Rendering {

    void CImmediateRenderer::Begin(const TRef<CCamera> &camera) {
        s_state.transform.view = camera->GetViewMatrix();
        s_state.transform.projection = camera->GetProjectionMatrix();
        s_state.transform.view_projection = camera->GetViewProjectionMatrix();

        // FIXME: This does not need to be set every time
        s_immediate_shader = CShaderLibrary::Get("immediate");

        if (!s_vertex_array) {
            s_vertex_buffer = CVertexBuffer::Create(nullptr, sizeof(s_data_buffer), EBufferUsage::DynamicDraw);
            s_vertex_buffer->SetLayout(SVertexImmediate::GetBufferLayout());
            s_vertex_array = CVertexArray::Create();
            s_vertex_array->AddVertexBuffer(s_vertex_buffer);
        }

        s_state.vertex_offset = 0;
    }

    void CImmediateRenderer::DrawCube(SVec3 center, SVec3 size, SVec4 color) {
        SVec3 half_size = size * 0.5f;

        // Forward
        AddVertex(SVec3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);

        // Right
        AddVertex(SVec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);

        // Back
        AddVertex(SVec3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);

        // Left
        AddVertex(SVec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);

        // Up
        AddVertex(SVec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);

        // Down
        AddVertex(SVec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        AddVertex(SVec3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
    }

    void CImmediateRenderer::AddVertex(SVec3 position, SVec3 normal, SVec2 uv, SVec4 color) {
        u32 vertex_offset = s_state.vertex_offset;
        HYP_ASSERT_MESSAGE(vertex_offset < 2000, "Immediate vertex buffer is full!");

        s_data_buffer[vertex_offset].position = position;
        s_data_buffer[vertex_offset].normal = normal;
        s_data_buffer[vertex_offset].uv = uv;
        s_data_buffer[vertex_offset].color = color;

        s_state.vertex_offset++;
    }

    void CImmediateRenderer::End() {
        s_immediate_shader->Bind();
        s_immediate_shader->SetMat4("u_transform.view", s_state.transform.view);
        s_immediate_shader->SetMat4("u_transform.projection", s_state.transform.projection);

        s_vertex_buffer->SetData(0, (u8 *)s_data_buffer, s_state.vertex_offset * sizeof(SVertexImmediate));
        s_vertex_array->Bind();

        CRenderCommand::Draw(EPrimitive::Triangles, s_state.vertex_offset, 0);
    }

}