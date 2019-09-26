#include "hyppch.hpp"

#include "hyperion/rendering/renderer.hpp"

#include "hyperion/rendering/render_command.hpp"

using namespace Hyperion::Math;

namespace Hyperion::Rendering {

    TRef<CVertexArray> g_cube_vertex_array;

    void CRenderer::Begin(const TRef<CCamera> &camera) {
        s_state->transform.view = camera->GetViewMatrix();
        s_state->transform.projection = camera->GetProjectionMatrix();
        s_state->transform.view_projection = camera->GetViewProjectionMatrix();

        // HACK: This should be set in a different place
        m_immediate_shader = CShaderLibrary::Get("immediate");
    }

    void CRenderer::End() {
        
    }

    void CRenderer::Submit(const TRef<CMesh> &mesh, const TRef<CShader> &shader, const Math::SMat4 &transform) {
        shader->Bind();
        shader->SetMat4("u_transform.view", s_state->transform.view);
        shader->SetMat4("u_transform.projection", s_state->transform.projection);
        shader->SetMat4("u_transform.model", transform);

        const TRef<CVertexArray> &vertex_array = mesh->GetVertexArray();
        vertex_array->Bind();

        EIndexFormat format = vertex_array->GetIndexBuffer()->GetFormat();
        for (const SSubMesh &sub_mesh : mesh->GetSubMeshes()) {
            CRenderCommand::DrawIndexed(format, sub_mesh.index_count, sub_mesh.index_offset, sub_mesh.vertex_offset);
        }

        vertex_array->Unbind();
    }

    void CRenderer::ImmediateCube(Math::SVec3 center, Math::SVec3 size, Math::SVec4 color) {
        // FIXME: This does obviously not work when we get called multiple times.
        // We should instead implement a global immedaiate vertex buffer we stream data into each frame.
        // If we do that it would probably be easier to do vertex instead of index rendering.
        
        m_immediate_shader->Bind();
        m_immediate_shader->SetMat4("u_transform.view", s_state->transform.view);
        m_immediate_shader->SetMat4("u_transform.projection", s_state->transform.projection);
        m_immediate_shader->SetFloat4("u_color", color);

        if (g_cube_vertex_array == nullptr) {
            SVec3 half_size = size * 0.5f;

            SVertexPNU verticies[8];

            verticies[0].position = SVec3(center.x + half_size.x, center.y - half_size.y, center.z - half_size.z);
            verticies[1].position = SVec3(center.x + half_size.x, center.y + half_size.y, center.z - half_size.z);
            verticies[2].position = SVec3(center.x - half_size.x, center.y + half_size.y, center.z - half_size.z);
            verticies[3].position = SVec3(center.x - half_size.x, center.y - half_size.y, center.z - half_size.z);

            verticies[4].position = SVec3(center.x - half_size.x, center.y - half_size.y, center.z + half_size.z);
            verticies[5].position = SVec3(center.x - half_size.x, center.y + half_size.y, center.z + half_size.z);
            verticies[6].position = SVec3(center.x + half_size.x, center.y + half_size.y, center.z + half_size.z);
            verticies[7].position = SVec3(center.x + half_size.x, center.y - half_size.y, center.z + half_size.z);

            TRef<CVertexBuffer> vertex_buffer = CVertexBuffer::Create((const u8 *)verticies, sizeof(verticies));
            vertex_buffer->SetLayout(SVertexPNU::GetBufferLayout());

            TVector<u16> indicies = {
                // Forward
                0, 1, 2,
                0, 2, 3,

                // Right
                7, 6, 1,
                7, 1, 0,

                // Back,
                4, 5, 6,
                4, 6, 7,

                // Left
                3, 2, 5,
                3, 5, 4,

                // Up
                5, 2, 1,
                5, 1, 6,

                // Down
                3, 4, 7,
                3, 7, 0
            };

            TRef<CIndexBuffer> index_buffer = CIndexBuffer::Create(indicies.data(), (u32)indicies.size());

            TRef<CVertexArray> vertex_array = CVertexArray::Create();
            vertex_array->AddVertexBuffer(vertex_buffer);
            vertex_array->SetIndexBuffer(index_buffer);
            vertex_array->Bind();

            g_cube_vertex_array = vertex_array;
        }

        g_cube_vertex_array->Bind();
        CRenderCommand::DrawIndexed(EIndexFormat::UInt16, g_cube_vertex_array->GetIndexBuffer()->GetCount(), 0, 0);
    }

}