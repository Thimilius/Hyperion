#include "hyppch.hpp"

#include "hyperion/rendering/renderer.hpp"

#include "hyperion/assets/asset_library.hpp"
#include "hyperion/assets/mesh_factory.hpp"
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

        PrepareShader(s_skybox.shader);
        skybox->Bind(0);
        DrawMesh(s_skybox.mesh);

        RenderEngine::GetRasterizerState()->SetCullingEnabled(culling_enabled);
    }

    void Renderer::DrawWorld(World *world) {
        auto &root_entities = world->GetRootEntites();
        Vector<MeshRendererComponent*> renderers;
        for (Entity *root : root_entities) {
            FindRendererComponents(root, renderers);
        }

        for (MeshRendererComponent *renderer : renderers) {
            Ref<Material> material = renderer->GetMaterial();
            material->SetVec3("u_camera.position", s_state.camera->GetPosition());
            
            DrawMesh(renderer->GetMesh(), material, renderer->GetTransform()->GetLocalToWorldMatrix());
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
        shader->Bind();
        shader->SetMat4("u_transform.view", s_state.transform.view);
        shader->SetMat4("u_transform.projection", s_state.transform.projection);
        shader->SetMat4("u_transform.model", transform);
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

    void Renderer::FindRendererComponents(Entity *parent, Vector<MeshRendererComponent*> &renderers) {
        MeshRendererComponent *renderer = parent->GetComponent<MeshRendererComponent>();
        if (renderer) {
            renderers.push_back(renderer);
        }

        TransformComponent *transform = parent->GetTransform();
        for (u32 i = 0; i < transform->GetChildCount(); i++) {
            FindRendererComponents(transform->GetChild(i)->GetEntity(), renderers);
        }
    }

}