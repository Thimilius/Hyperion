﻿#include <hyperion/hyperion.hpp>
#include <hyperion/entry_point.hpp>

using namespace Hyperion;
using namespace Hyperion::Rendering;
using namespace Hyperion::Audio;
using namespace Hyperion::Editor;

class SandboxApp : public Application {
public:
    SandboxApp(const ApplicationSettings &settings) : Application(settings) { }
protected:
    Ref<Camera> m_camera;
    Ref<RenderTexture> m_render_texture;

    Ref<Mesh> m_cube_mesh;
    Ref<Material> m_cube_material;
    Ref<Texture2D> m_cube_texture;
    Ref<TextureCubemap> m_skybox_texture;

    Ref<Mesh> m_light_mesh;
    Ref<Material> m_light_material;
    Ref<Texture2D> m_light_texture;

    Entity *m_entity_parent;
    Entity *m_entity_child;

    void OnInit() override {
        EditorLayer *editor_layer = new EditorLayer();
        PushLayer(editor_layer);
        
        m_camera = editor_layer->GetCamera();

        m_render_texture = RenderTexture::Create(GetWindow()->GetWidth(), GetWindow()->GetHeight(), RenderTextureFormat::RGBA8);
        
        m_cube_mesh = MeshFactory::CreateCube(1);
        m_cube_material = Material::Create(AssetLibrary::GetShader("phong"));
        m_cube_texture = AssetLibrary::GetTexture2D("grass");
        m_skybox_texture = AssetLibrary::LoadTextureCubemap("skybox", "data/textures/galaxy", ".png");

        m_light_mesh = MeshFactory::CreatePlane(0.25f, 0.25f);
        m_light_material = Material::Create(AssetLibrary::GetShader("texture"));
        m_light_texture = AssetLibrary::GetTexture2D("light_icon");
        m_light_texture->SetAnisotropicFilter(TextureAnisotropicFilter::Times16);

        m_entity_parent = Entity::Create("Entity_0_0");
        m_entity_parent->GetTransform()->SetPosition(Vec3(0, 0, 0));
        auto mesh_renderer = m_entity_parent->AddComponent<MeshRendererComponent>();
        mesh_renderer->SetMesh(m_cube_mesh);
        mesh_renderer->SetMaterial(m_cube_material);

        m_entity_child = Entity::Create(StringUtils::Format("Entity_1_0"));
        m_entity_child->GetTransform()->SetPosition(Vec3(2, 0, 0));
        m_entity_child->GetTransform()->SetParent(m_entity_parent->GetTransform());
        mesh_renderer = m_entity_child->AddComponent<MeshRendererComponent>();
        mesh_renderer->SetMesh(m_cube_mesh);
        mesh_renderer->SetMaterial(m_cube_material);
    }
    
    void OnEvent(Event &event) override {
        if (event.GetType() == EventType::WindowResize) {
            m_render_texture->Resize(GetWindow()->GetWidth(), GetWindow()->GetHeight());
        }
    }

    void OnUpdate(f32 delta_time) override {

    }

    void OnRender() override {
        RenderEngine::SetActiveRenderTarget(m_render_texture);
        
        RenderEngine::Clear(ClearMask::Color | ClearMask::Depth, Color::Black());

        Vec3 light_position = Vec3(1.5f, 2.0f, 3.0f);
        Color light_color = Color::White();

        Renderer::Begin(m_camera);
        {
            Renderer::DrawSkybox(m_skybox_texture);

            RenderEngine::Clear(ClearMask::Depth);

            // Draw entites
            {
                m_cube_material->SetVec3("u_camera.position", m_camera->GetPosition());
                m_cube_material->SetVec3("u_light.position", light_position);
                m_cube_material->SetTexture2D("u_texture", m_cube_texture);

                m_cube_material->SetColor("u_light.color", light_color);
                Renderer::Draw(m_cube_mesh, m_cube_material, m_entity_parent->GetTransform()->GetLocalToWorldMatrix());

                m_cube_material->SetColor("u_light.color", Color::Red());
                Renderer::Draw(m_cube_mesh, m_cube_material, m_entity_child->GetTransform()->GetLocalToWorldMatrix());
            }

            // Draw light icon
            {
                m_light_material->SetTexture2D("u_texture", m_light_texture);
                m_light_material->SetColor("u_color", light_color);

                RenderEngine::GetRasterizerState()->SetCullingEnabled(false);
                RenderEngine::GetRasterizerState()->SetBlendingEnabled(true);
                RenderEngine::GetRasterizerState()->SetDepthMaskEnabled(false);
                Renderer::Draw(m_light_mesh, m_light_material, Mat4::Translate(light_position) * Mat4::Rotate(Vec3(1, 0, 0), 90));
                RenderEngine::GetRasterizerState()->SetCullingEnabled(true);
                RenderEngine::GetRasterizerState()->SetBlendingEnabled(false);
                RenderEngine::GetRasterizerState()->SetDepthMaskEnabled(true);
            }
        }
        Renderer::End();

        RenderEngine::Blit(nullptr, m_render_texture);
        // Setting back buffer as render target before swapping buffers fixes vsync
        RenderEngine::SetActiveRenderTarget(nullptr);
    }
    
    void OnTick() override {

    }
};

Hyperion::Application *Hyperion::CreateApplication() {
    ApplicationSettings settings;
    settings.audio.backend = AudioBackend::None;
    return new SandboxApp(settings);
}