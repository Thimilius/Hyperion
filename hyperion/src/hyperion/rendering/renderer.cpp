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
            CRenderCommand::DrawIndexed(EPrimitive::Triangles, format, sub_mesh.index_count, sub_mesh.index_offset, sub_mesh.vertex_offset);
        }

        vertex_array->Unbind();
    }

    void CRenderer::End() {
        
    }

}