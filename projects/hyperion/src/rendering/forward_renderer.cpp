#include "hyppch.hpp"

#include "hyperion/rendering/forward_renderer.hpp"

#include "hyperion/assets/asset_library.hpp"
#include "hyperion/assets/mesh_factory.hpp"
#include "hyperion/entity/components/transform.hpp"
#include "hyperion/entity/components/mesh_renderer.hpp"
#include "hyperion/entity/components/light.hpp"

namespace Hyperion::Rendering {

    void ForwardRenderer::Begin(const CameraData &camera) {
        s_state.transform.view = camera.view_matrix;
        s_state.transform.projection = camera.projection_matrix;
        s_state.transform.view_projection = camera.view_projection_matrix;

        s_state.camera = camera;
    }

    void ForwardRenderer::DrawSkybox(const Ref<TextureCubemap> &skybox) {
        bool culling_enabled = RenderEngine::GetRasterizerState()->IsCullingEnabled();
        RenderEngine::GetRasterizerState()->SetCullingEnabled(false);
        DepthEquation depth_equation = RenderEngine::GetRasterizerState()->GetDepthEquation();
        RenderEngine::GetRasterizerState()->SetDepthEquation(DepthEquation::LessEqual);

        s_skybox.shader->Bind();
        s_skybox.shader->SetMat4("u_transform.view", s_state.transform.view);
        s_skybox.shader->SetMat4("u_transform.projection", s_state.transform.projection);
        s_skybox.shader->SetInt("u_skybox", 0);
        skybox->Bind(0);
        DrawCall(s_skybox.mesh);

        RenderEngine::GetRasterizerState()->SetDepthEquation(depth_equation);
        RenderEngine::GetRasterizerState()->SetCullingEnabled(culling_enabled);
    }

    void ForwardRenderer::DrawWorld(World *world) {
        WorldEnvironment environment = world->GetEnvironment();
        RenderEngine::Clear(ClearMask::Color | ClearMask::Depth | ClearMask::Stencil, environment.background_color);
        
        auto &renderers = world->GetMeshRenderers();
        auto &lights = world->GetLights();

        // Set the main light to be the first directional light we can find
        auto main_light_pos = std::find_if(lights.begin(), lights.end(), [](Light *light) { return light->GetLightType() == LightType::Directional; });
        Light *main_light = main_light_pos != lights.end() ? *main_light_pos : nullptr;

        for (MeshRenderer *renderer : renderers) {
            if (!renderer->IsEnabled()) {
                continue;
            }

            Transform *transform = renderer->GetTransform();
            DrawMesh(renderer->GetSharedMesh(), renderer->GetSharedMaterial(), transform->GetLocalToWorldMatrix(), transform->GetWorldToLocalMatrix(), main_light);
        }

        if (environment.background_mode == WorldEnvironmentBackgroundMode::Skybox) {
            if (environment.background_skybox != nullptr) {
                DrawSkybox(environment.background_skybox);
            }
        }
    }

    void ForwardRenderer::DrawMesh(const Ref<Mesh> &mesh, const Ref<Material> &material, const Mat4 &transform) {
        DrawMesh(mesh, material, transform, transform.Inverted(), nullptr);
    }

    void ForwardRenderer::DrawMesh(const Ref<Mesh> &mesh, const Ref<Material> &material, const Mat4 &transform, const Mat4 &inverse_transform, Light *main_light) {
        PrepareShader(material->GetShader(), transform, inverse_transform, main_light);
        material->Bind();
        DrawCall(mesh);
    }

    void ForwardRenderer::End() {
        
    }

    void ForwardRenderer::Init() {
        s_skybox.shader = AssetLibrary::GetShader("standard_skybox");
        s_skybox.mesh = MeshFactory::CreateCube(1);
    }

    void ForwardRenderer::PrepareShader(const Ref<Shader> &shader, const Mat4 &transform, const Mat4 &inverse_transform, Light *main_light) {
        shader->Bind();
        
        shader->SetMat4("u_transform.mvp", s_state.transform.view_projection * transform);

        if (shader->GetProperties().light_mode == ShaderLightMode::Forward) {
            // TODO: There needs to be a better way of "not setting" shader uniforms
            // when we are not sure if the shader has them to avoid unnecessary log messages
            shader->SetMat4("u_transform.model", transform);
            shader->SetMat3("u_transform.model_normal", Mat3(inverse_transform.Transposed()));
            shader->SetVec3("u_camera.position", s_state.camera.position);

            // FIXME: Correctly set up properties for main and additional lights
            if (main_light) {
                shader->SetFloat("u_main_light.intensity", main_light->GetIntensity());
                shader->SetVec4("u_main_light.color", main_light->GetColor());
                shader->SetVec3("u_main_light.direction", main_light->GetTransform()->GetForward());
            }
        }
    }

    void ForwardRenderer::DrawCall(const Ref<Mesh> &mesh) {
        const Ref<VertexArray> &vertex_array = mesh->GetVertexArray();
        vertex_array->Bind();

        IndexFormat format = vertex_array->GetIndexBuffer()->GetFormat();
        for (const SubMesh &sub_mesh : mesh->GetSubMeshes()) {
            RenderEngine::DrawIndexed(sub_mesh.topology, format, sub_mesh.index_count, sub_mesh.index_offset, sub_mesh.vertex_offset);
        }

        vertex_array->Unbind();
    }

}