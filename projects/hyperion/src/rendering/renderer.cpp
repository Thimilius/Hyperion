#include "hyppch.hpp"

#include "hyperion/rendering/renderer.hpp"

#include "hyperion/assets/asset_library.hpp"
#include "hyperion/assets/mesh_factory.hpp"

namespace Hyperion::Rendering {

    void Renderer::Begin(const Ref<Camera> &camera) {
        s_state.transform.view = camera->GetViewMatrix();
        s_state.transform.projection = camera->GetProjectionMatrix();
        s_state.transform.view_projection = camera->GetViewProjectionMatrix();
    }

    void Renderer::DrawSkybox(const Ref<TextureCubemap> &skybox) {
        // TODO: Add a push and pop behaviour to rasterizer state
        RenderEngine::GetRasterizerState()->DisableFeature(RenderFeature::Culling);

        PrepareShader(s_skybox.shader);
        skybox->Bind(0);
        Draw(s_skybox.mesh);

        RenderEngine::GetRasterizerState()->EnableFeature(RenderFeature::Culling);
    }

    void Renderer::Draw(const Ref<Mesh> &mesh, const Ref<Shader> &shader, const Mat4 &transform) {
        PrepareShader(shader, transform);
        Draw(mesh);
    }

    void Renderer::End() {
        
    }

    void Renderer::Init() {
        s_skybox.shader = AssetLibrary::GetShader("skybox");
        s_skybox.mesh = MeshFactory::CreateCube(1);
    }

    void Renderer::PrepareShader(const Ref<Shader> &shader) {
        shader->Bind();
        shader->SetMat4("u_transform.view", s_state.transform.view);
        shader->SetMat4("u_transform.projection", s_state.transform.projection);
    }

    void Renderer::PrepareShader(const Ref<Shader> &shader, const Mat4 &transform) {
        shader->Bind();
        shader->SetMat4("u_transform.view", s_state.transform.view);
        shader->SetMat4("u_transform.projection", s_state.transform.projection);
        shader->SetMat4("u_transform.model", transform);
    }

    void Renderer::Draw(const Ref<Mesh> &mesh) {
        const Ref<VertexArray> &vertex_array = mesh->GetVertexArray();
        vertex_array->Bind();

        IndexFormat format = vertex_array->GetIndexBuffer()->GetFormat();
        for (const SubMesh &sub_mesh : mesh->GetSubMeshes()) {
            RenderEngine::DrawIndexed(PrimitiveType::Triangles, format, sub_mesh.index_count, sub_mesh.index_offset, sub_mesh.vertex_offset);
        }

        vertex_array->Unbind();
    }

}