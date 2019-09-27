#include "hyppch.hpp"

#include "hyperion/rendering/renderer.hpp"

#include "hyperion/rendering/render_command.hpp"

using namespace Hyperion::Math;

namespace Hyperion::Rendering {

    void CRenderer::Begin(const TRef<CCamera> &camera) {
        s_state.transform.view = camera->GetViewMatrix();
        s_state.transform.projection = camera->GetProjectionMatrix();
        s_state.transform.view_projection = camera->GetViewProjectionMatrix();
    }

    void CRenderer::Submit(const TRef<CMesh> &mesh, const TRef<CShader> &shader, const Math::SMat4 &transform) {
        shader->Bind();
        shader->SetMat4("u_transform.view", s_state.transform.view);
        shader->SetMat4("u_transform.projection", s_state.transform.projection);
        shader->SetMat4("u_transform.model", transform);

        const TRef<CVertexArray> &vertex_array = mesh->GetVertexArray();
        vertex_array->Bind();

        EIndexFormat format = vertex_array->GetIndexBuffer()->GetFormat();
        for (const SSubMesh &sub_mesh : mesh->GetSubMeshes()) {
            CRenderCommand::DrawIndexed(format, sub_mesh.index_count, sub_mesh.index_offset, sub_mesh.vertex_offset);
        }

        vertex_array->Unbind();
    }

    void CRenderer::End() {
        
    }

    void CRenderer::ImmediateBegin(const TRef<CCamera> &camera) {
        s_immediate_state.transform.view = camera->GetViewMatrix();
        s_immediate_state.transform.projection = camera->GetProjectionMatrix();
        s_immediate_state.transform.view_projection = camera->GetViewProjectionMatrix();

        // FIXME: This does not need to be set every time
        s_immediate_shader = CShaderLibrary::Get("immediate");

        if (!s_immediate_vertex_array) {
            s_immediate_vertex_buffer = CVertexBuffer::Create(nullptr, sizeof(s_immediate_buffer), EBufferUsage::DynamicDraw);
            s_immediate_vertex_buffer->SetLayout(SVertexPNCU::GetBufferLayout());
            s_immediate_vertex_array = CVertexArray::Create();
            s_immediate_vertex_array->AddVertexBuffer(s_immediate_vertex_buffer);
        }

        s_immediate_vertex_offset = 0;
    }

    void CRenderer::ImmediateCube(Math::SVec3 center, Math::SVec3 size, Math::SVec4 color) {
        SVec3 half_size = size * 0.5f;

        // Forward
        ImmediateVertex(SVec3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);

        // Right
        ImmediateVertex(SVec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);

        // Back
        ImmediateVertex(SVec3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);

        // Left
        ImmediateVertex(SVec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);

        // Up
        ImmediateVertex(SVec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);

        // Down
        ImmediateVertex(SVec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z), SVec3(), SVec2(), color);
        ImmediateVertex(SVec3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z), SVec3(), SVec2(), color);
    }

    void CRenderer::ImmediateVertex(Math::SVec3 position, Math::SVec3 normal, Math::SVec2 uv, Math::SVec4 color) {
        HYP_ASSERT_MESSAGE(s_immediate_vertex_offset < 2000, "Immediate vertex buffer is full!");

        s_immediate_buffer[s_immediate_vertex_offset].position = position;
        s_immediate_buffer[s_immediate_vertex_offset].normal = normal;
        s_immediate_buffer[s_immediate_vertex_offset].uv = uv;
        s_immediate_buffer[s_immediate_vertex_offset].color = color;

        s_immediate_vertex_offset++;
    }

    void CRenderer::ImmediateEnd() {
        s_immediate_shader->Bind();
        s_immediate_shader->SetMat4("u_transform.view", s_immediate_state.transform.view);
        s_immediate_shader->SetMat4("u_transform.projection", s_immediate_state.transform.projection);

        s_immediate_vertex_buffer->SetData(0, (u8*)s_immediate_buffer, s_immediate_vertex_offset * sizeof(SVertexPNU));
        s_immediate_vertex_array->Bind();

        CRenderCommand::Draw(s_immediate_vertex_offset, 0);
    }

}