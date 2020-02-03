#include "hyppch.hpp"

#include "hyperion/rendering/renderer.hpp"

#include "hyperion/assets/asset_library.hpp"
#include "hyperion/assets/mesh_factory.hpp"
#include "hyperion/entity/components/transform_component.hpp"
#include "hyperion/entity/components/mesh_renderer_component.hpp"

namespace Hyperion::Rendering {

    void Renderer::Begin(const Ref<Camera> &camera) {
        s_state.transform.view = camera->GetViewMatrix();
        s_state.transform.projection = camera->GetProjectionMatrix();
        s_state.transform.view_projection = camera->GetViewProjectionMatrix();

        s_state.camera = camera;
    }

    void Renderer::DrawSkybox(const Ref<TextureCubemap> &skybox) {
        bool culling_enabled = RenderEngine::GetRasterizerState()->IsCullingEnabled();
        RenderEngine::GetRasterizerState()->SetCullingEnabled(false);
        DepthEquation depth_equation = RenderEngine::GetRasterizerState()->GetDepthEquation();
        RenderEngine::GetRasterizerState()->SetDepthEquation(DepthEquation::LessEqual);

        PrepareShader(s_skybox.shader);
        s_skybox.shader->SetInt("u_skybox", 0);
        skybox->Bind(0);
        DrawMesh(s_skybox.mesh);

        RenderEngine::GetRasterizerState()->SetDepthEquation(depth_equation);
        RenderEngine::GetRasterizerState()->SetCullingEnabled(culling_enabled);
    }

    void Renderer::DrawWorld(World *world) {
        auto &renderers = world->GetMeshRenderers();
        for (MeshRendererComponent *renderer : renderers) {
            if (!renderer->IsEnabled()) {
                continue;
            }

            DrawMesh(renderer->GetSharedMesh(), renderer->GetSharedMaterial(), renderer->GetTransform()->GetLocalToWorldMatrix());
        }
    }

    void Renderer::DrawMesh(const Ref<Mesh> &mesh, const Ref<Material> &material, const Mat4 &transform) {
        PrepareShader(material->GetShader(), transform);
        material->Bind();
        DrawMesh(mesh);
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
        PrepareShader(shader);

        shader->SetMat4("u_transform.model", transform);

        shader->SetFloat3("u_camera.position", s_state.camera->GetPosition());
    }

    void Renderer::DrawMesh(const Ref<Mesh> &mesh) {
        const Ref<VertexArray> &vertex_array = mesh->GetVertexArray();
        vertex_array->Bind();

        IndexFormat format = vertex_array->GetIndexBuffer()->GetFormat();
        for (const SubMesh &sub_mesh : mesh->GetSubMeshes()) {
            RenderEngine::DrawIndexed(PrimitiveType::Triangles, format, sub_mesh.index_count, sub_mesh.index_offset, sub_mesh.vertex_offset);
        }

        vertex_array->Unbind();
    }

}