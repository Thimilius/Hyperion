#include "hyppch.hpp"

#include "hyperion/rendering/renderer.hpp"

namespace Hyperion::Rendering {

    void Renderer::Begin(const Ref<Camera> &camera) {
        s_state.transform.view = camera->GetViewMatrix();
        s_state.transform.projection = camera->GetProjectionMatrix();
        s_state.transform.view_projection = camera->GetViewProjectionMatrix();
    }

    void Renderer::Submit(const Ref<Mesh> &mesh, const Ref<Shader> &shader, const Mat4 &transform) {
        shader->Bind();
        shader->SetMat4("u_transform.view", s_state.transform.view);
        shader->SetMat4("u_transform.projection", s_state.transform.projection);
        shader->SetMat4("u_transform.model", transform);

        const Ref<VertexArray> &vertex_array = mesh->GetVertexArray();
        vertex_array->Bind();

        IndexFormat format = vertex_array->GetIndexBuffer()->GetFormat();
        for (const SubMesh &sub_mesh : mesh->GetSubMeshes()) {
            RenderEngine::DrawIndexed(Primitive::Triangles, format, sub_mesh.index_count, sub_mesh.index_offset, sub_mesh.vertex_offset);
        }

        vertex_array->Unbind();
    }

    void Renderer::End() {
        
    }

}