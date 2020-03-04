#include "hyppch.hpp"

#include "hyperion/rendering/forward_renderer.hpp"

#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/assets/mesh_factory.hpp"
#include "hyperion/entity/components/transform.hpp"
#include "hyperion/entity/components/rendering/mesh_renderer.hpp"
#include "hyperion/entity/components/rendering/light.hpp"

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
        RenderEngine::Clear(ClearMask::Color | ClearMask::Depth | ClearMask::Stencil, environment.background.color);
        
        const Vector<MeshRenderer *> &renderers = world->GetMeshRenderers();
        const Vector<Light *> &lights = world->GetLights();

        f32 intensity = environment.ambient_light.intensity;
        Color color = environment.ambient_light.color;
        s_state.lighting.ambient_color = Vec3(intensity * color.r, intensity * color.g, intensity * color.b);

        // Set the main light to be the first directional light we can find
        auto main_light_pos = std::find_if(lights.begin(), lights.end(), [](Light *light) {
            return light->IsEnabled() && light->GetLightType() == LightType::Directional;
        });
        s_state.lighting.main_light = main_light_pos != lights.end() ? *main_light_pos : nullptr;

        Vector<Light *> point_lights;
        s_state.lighting.point_lights = &point_lights;
        for (Light *light : lights) {
            if (light->IsEnabled() && light->GetLightType() == LightType::Point) {
                point_lights.push_back(light);
            }
        }

        for (MeshRenderer *renderer : renderers) {
            if (!renderer->IsEnabled()) {
                continue;
            }

            Vec3 position = renderer->GetTransform()->GetPosition();
            std::sort(point_lights.begin(), point_lights.end(), [position](Light *first, Light *second) {
                f32 first_distance = (first->GetTransform()->GetPosition() - position).SqrMagnitude();
                f32 second_distance = (second->GetTransform()->GetPosition() - position).SqrMagnitude();
                return first_distance < second_distance;
            });

            Transform *transform = renderer->GetTransform();
            DrawMesh(renderer->GetRenderMesh(), renderer->GetRenderMaterial(), transform->GetLocalToWorldMatrix(), transform->GetWorldToLocalMatrix());
        }

        if (environment.background.mode == WorldEnvironment::Background::Mode::Skybox) {
            if (environment.background.skybox != nullptr) {
                DrawSkybox(environment.background.skybox);
            }
        }
    }

    void ForwardRenderer::DrawMesh(const Ref<Mesh> &mesh, const Ref<Material> &material, const Mat4 &transform) {
        DrawMesh(mesh, material, transform, transform.Inverted());
    }

    void ForwardRenderer::DrawMesh(const Ref<Mesh> &mesh, const Ref<Material> &material, const Mat4 &transform, const Mat4 &inverse_transform) {
        PrepareShader(material->GetShader(), transform, inverse_transform);
        material->Bind();
        DrawCall(mesh);
    }

    void ForwardRenderer::End() {
        
    }

    void ForwardRenderer::Init() {
        s_skybox.shader = AssetManager::GetShader("standard_skybox");
        s_skybox.mesh = MeshFactory::CreateCube(1);

        for (u32 i = 0; i < s_state.lighting.MAX_POINT_LIGHT_COUNT; i++) {
            s_state.lighting.point_light_uniforms[i].intensity = StringUtils::Format("u_lighting.point_lights[{}].intensity", i);
            s_state.lighting.point_light_uniforms[i].color = StringUtils::Format("u_lighting.point_lights[{}].color", i);
            s_state.lighting.point_light_uniforms[i].position = StringUtils::Format("u_lighting.point_lights[{}].position", i);
            s_state.lighting.point_light_uniforms[i].range = StringUtils::Format("u_lighting.point_lights[{}].range", i);
        }
    }

    void ForwardRenderer::PrepareShader(const Ref<Shader> &shader, const Mat4 &transform, const Mat4 &inverse_transform) {
        shader->Bind();
        
        shader->SetMat4("u_transform.mvp", s_state.transform.view_projection * transform);

        if (shader->GetProperties().light_mode == ShaderLightMode::Forward) {
            // TODO: There needs to be a better way of "not setting" shader uniforms
            // when we are not sure if the shader has them to avoid unnecessary log messages
            shader->SetMat4("u_transform.model", transform);
            shader->SetMat3("u_transform.model_normal", Mat3(inverse_transform.Transposed()));
            shader->SetVec3("u_camera.position", s_state.camera.position);

            // Setup lighting
            {
                State::Lighting &lighting = s_state.lighting;

                shader->SetVec3("u_lighting.ambient_color", lighting.ambient_color);

                Light *main_light = lighting.main_light;
                if (main_light) {
                    shader->SetFloat("u_lighting.main_light.intensity", main_light->GetIntensity());
                    shader->SetVec4("u_lighting.main_light.color", main_light->GetColor());
                    shader->SetVec3("u_lighting.main_light.direction", main_light->GetTransform()->GetForward());
                } else {
                    shader->SetVec4("u_lighting.main_light.color", Color::Black());
                }

                if (lighting.point_lights) {
                    u32 full_point_light_count = (u32)lighting.point_lights->size();
                    u32 max_point_light_count = lighting.MAX_POINT_LIGHT_COUNT;
                    u32 light_count = full_point_light_count <= max_point_light_count ? full_point_light_count : max_point_light_count;
                    shader->SetInt("u_lighting.point_light_count", light_count);
                    for (u32 i = 0; i < light_count && i < lighting.MAX_POINT_LIGHT_COUNT; i++) {
                        Light *light = lighting.point_lights->at(i);
                        shader->SetFloat(lighting.point_light_uniforms[i].intensity, light->GetIntensity());
                        shader->SetVec4(lighting.point_light_uniforms[i].color, light->GetColor());
                        shader->SetVec3(lighting.point_light_uniforms[i].position, light->GetTransform()->GetPosition());
                        shader->SetFloat(lighting.point_light_uniforms[i].range, light->GetRange());
                    }
                }
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