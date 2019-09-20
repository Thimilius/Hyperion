#include "hyppch.hpp"

#include "hyperion/rendering/renderer.hpp"

namespace Hyperion::Rendering {

    using namespace Math;

    void CRenderer::Begin(const CCamera &camera) {
        s_state->transform.view = camera.GetViewMatrix();
        s_state->transform.projection = camera.GetProjectionMatrix();
        s_state->transform.view_projection = camera.GetViewProjectionMatrix();
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
        CRenderCommand::DrawIndexed(vertex_array->GetIndexBuffer());
    }

}