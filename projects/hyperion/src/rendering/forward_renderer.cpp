#include "hyppch.hpp"

#include "hyperion/rendering/forward_renderer.hpp"

#include "hyperion/rendering/render_command.hpp"
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/assets/mesh_factory.hpp"
#include "hyperion/entity/world.hpp"
#include "hyperion/entity/components/transform.hpp"
#include "hyperion/entity/components/rendering/mesh_renderer.hpp"
#include "hyperion/entity/components/rendering/light.hpp"

namespace Hyperion::Rendering {

    void ForwardRenderer::SetCameraData(const CameraData &camera) {
        s_state.transform.view = camera.view_matrix;
        s_state.transform.projection = camera.projection_matrix;
        s_state.transform.view_projection = camera.view_projection_matrix;

        s_state.camera = camera;
    }

    void ForwardRenderer::DrawSkybox(TextureCubemap *skybox) {
        bool culling_enabled = RenderCommand::GetRasterizerState()->IsCullingEnabled();
        RenderCommand::GetRasterizerState()->SetCullingEnabled(false);
        DepthEquation depth_equation = RenderCommand::GetRasterizerState()->GetDepthEquation();
        RenderCommand::GetRasterizerState()->SetDepthEquation(DepthEquation::LessEqual);

        s_skybox_resources.shader->Bind();
        s_skybox_resources.shader->SetMat4("u_transform.view", s_state.transform.view);
        s_skybox_resources.shader->SetMat4("u_transform.projection", s_state.transform.projection);
        s_skybox_resources.shader->SetInt("u_skybox", 0);
        skybox->Bind(0);
        DrawCall(s_skybox_resources.mesh);

        RenderCommand::GetRasterizerState()->SetDepthEquation(depth_equation);
        RenderCommand::GetRasterizerState()->SetCullingEnabled(culling_enabled);
    }

    void ForwardRenderer::DrawEntities(World *world, LayerMask layers) {
        const Vector<MeshRenderer *> &world_renderers = world->GetMeshRenderers();
        Set<MeshRenderer *> renderers;
        std::copy_if(world_renderers.begin(), world_renderers.end(), std::inserter(renderers, renderers.begin()), [layers](MeshRenderer *renderer) {
            if (!renderer->IsActiveAndEnabled() || !renderer->GetRenderMesh() || !renderer->GetRenderMaterial()) {
                return false;
            }

            LayerMask layer = renderer->GetEntity()->GetLayer();
            return (layer & layers) == layer;
        });

        WorldEnvironment environment = world->GetEnvironment();
        const Vector<Light *> &world_lights = world->GetLights();

        f32 intensity = environment.ambient_light.intensity;
        Color color = environment.ambient_light.color;
        s_state.lighting.ambient_color = Vec3(intensity * color.r, intensity * color.g, intensity * color.b);

        // Set the main light to be the first directional light we can find
        auto main_light_pos = std::find_if(world_lights.begin(), world_lights.end(), [](Light *light) {
            return light->IsActiveAndEnabled() && light->GetLightType() == LightType::Directional;
        });
        s_state.lighting.main_light = main_light_pos != world_lights.end() ? *main_light_pos : nullptr;

        Vector<Light *> point_lights;
        std::copy_if(world_lights.begin(), world_lights.end(), std::back_inserter(point_lights), [](Light *light) {
            return light->IsActiveAndEnabled() && light->GetLightType() == LightType::Point;
        });
        s_state.lighting.point_lights = &point_lights;

        for (MeshRenderer *renderer : renderers) {
            Vec3 position = renderer->GetTransform()->GetPosition();
            std::sort(point_lights.begin(), point_lights.end(), [position](Light *first, Light *second) {
                f32 first_distance = (first->GetTransform()->GetPosition() - position).SqrMagnitude();
                f32 second_distance = (second->GetTransform()->GetPosition() - position).SqrMagnitude();
                return first_distance < second_distance;
            });

            Transform *transform = renderer->GetTransform();
            DrawMesh(renderer->GetRenderMesh(), renderer->GetRenderMaterial(), transform->GetLocalToWorldMatrix(), transform->GetWorldToLocalMatrix());
        }
    }

    void ForwardRenderer::DrawMesh(Mesh *mesh, Material *material, const Mat4 &transform) {
        DrawMesh(mesh, material, transform, transform.Inverted());
    }

    void ForwardRenderer::DrawMesh(Mesh *mesh, Material *material, const Mat4 &transform, const Mat4 &inverse_transform) {
        PrepareMaterial(material, transform, inverse_transform);
        material->Bind();
        DrawCall(mesh);
    }

    void ForwardRenderer::Init() {
        for (u32 i = 0; i < s_state.lighting.MAX_POINT_LIGHT_COUNT; i++) {
            s_state.lighting.point_light_uniforms[i].intensity = StringUtils::Format("u_lighting.point_lights[{}].intensity", i);
            s_state.lighting.point_light_uniforms[i].color = StringUtils::Format("u_lighting.point_lights[{}].color", i);
            s_state.lighting.point_light_uniforms[i].position = StringUtils::Format("u_lighting.point_lights[{}].position", i);
            s_state.lighting.point_light_uniforms[i].range = StringUtils::Format("u_lighting.point_lights[{}].range", i);
        }

        s_skybox_resources.shader = AssetManager::GetShader("standard_skybox");
        s_skybox_resources.mesh = MeshFactory::CreateCube(1);
    }

    void ForwardRenderer::Shutdown() {
        
    }

    void ForwardRenderer::PrepareMaterial(Material *material, const Mat4 &transform, const Mat4 &inverse_transform) {
        material->SetMat4("u_transform.mvp", s_state.transform.view_projection * transform);

        if (material->GetShader()->GetAttributes().light_mode == ShaderLightMode::Forward) {
            // TODO: There needs to be a better way of "not setting" shader uniforms
            // when we are not sure if the shader has them to avoid unnecessary log messages
            material->SetMat4("u_transform.model", transform);
            material->SetMat3("u_transform.model_normal", Mat3(inverse_transform.Transposed()));
            material->SetVec3("u_camera.position", s_state.camera.position);

            // Setup lighting
            {
                State::Lighting &lighting = s_state.lighting;

                material->SetVec3("u_lighting.ambient_color", lighting.ambient_color);

                Light *main_light = lighting.main_light;
                if (main_light) {
                    material->SetFloat("u_lighting.main_light.intensity", main_light->GetIntensity());
                    material->SetVec4("u_lighting.main_light.color", main_light->GetColor());
                    material->SetVec3("u_lighting.main_light.direction", main_light->GetTransform()->GetForward());
                } else {
                    material->SetVec4("u_lighting.main_light.color", Color::Black());
                }

                if (lighting.point_lights) {
                    u32 full_point_light_count = static_cast<u32>(lighting.point_lights->size());
                    u32 max_point_light_count = lighting.MAX_POINT_LIGHT_COUNT;
                    u32 light_count = full_point_light_count <= max_point_light_count ? full_point_light_count : max_point_light_count;
                    material->SetInt("u_lighting.point_light_count", light_count);
                    for (u32 i = 0; i < light_count && i < lighting.MAX_POINT_LIGHT_COUNT; i++) {
                        Light *light = lighting.point_lights->at(i);
                        material->SetFloat(lighting.point_light_uniforms[i].intensity, light->GetIntensity());
                        material->SetVec4(lighting.point_light_uniforms[i].color, light->GetColor());
                        material->SetVec3(lighting.point_light_uniforms[i].position, light->GetTransform()->GetPosition());
                        material->SetFloat(lighting.point_light_uniforms[i].range, light->GetRange());
                    }
                }
            }
        }
    }

    void ForwardRenderer::DrawCall(Mesh *mesh) {
        VertexArray *vertex_array = mesh->GetVertexArray();
        vertex_array->Bind();

        IndexFormat format = vertex_array->GetIndexBuffer()->GetFormat();
        for (const SubMesh &sub_mesh : mesh->GetSubMeshes()) {
            RenderCommand::DrawIndexed(sub_mesh.topology, format, sub_mesh.index_count, sub_mesh.index_offset, sub_mesh.vertex_offset);
        }

        vertex_array->Unbind();
    }

}