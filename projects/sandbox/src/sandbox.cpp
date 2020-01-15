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

    void OnInit() override {
        EditorEngine::Init();
        m_camera = EditorEngine::GetCamera();

        m_render_texture = RenderTexture::Create(GetWindow()->GetWidth(), GetWindow()->GetHeight(), RenderTextureFormat::RGBA8);

        m_cube_mesh = MeshFactory::CreateCube(1);
        m_cube_material = Material::Create(AssetLibrary::GetShader("phong"));
        m_cube_texture = AssetLibrary::GetTexture2D("grass");
        m_skybox_texture = AssetLibrary::LoadTextureCubemap("skybox", "data/textures/galaxy", ".png");
    }
    
    void OnEvent(Event &event) override {
        if (event.GetType() == EventType::WindowResize) {
            m_render_texture->Resize(GetWindow()->GetWidth(), GetWindow()->GetHeight());
        }
    }

    void OnUpdate(f32 delta_time) override {
        EditorEngine::Update();
    }

    void OnRender() override {
        RenderEngine::SetActiveRenderTarget(m_render_texture);
        
        RenderEngine::Clear(ClearMask::Color | ClearMask::Depth, Color::Black());

        Renderer::Begin(m_camera);
        {
            Renderer::DrawSkybox(m_skybox_texture);

            RenderEngine::Clear(ClearMask::Depth);

            m_cube_material->SetVec3("u_camera.position", m_camera->GetPosition());
            m_cube_material->SetVec3("u_light.position", Vec3(1.5f, 2.0f, 3.0f));
            m_cube_material->SetVec3("u_light.color", Vec3::One());
            m_cube_material->SetTexture2D("u_texture", m_cube_texture);

            Renderer::Draw(m_cube_mesh, m_cube_material, Mat4::Identity());
        }
        Renderer::End();

        RenderEngine::Blit(nullptr, m_render_texture);
        // Setting back buffer as render target before swapping buffers fixes vsync
        RenderEngine::SetActiveRenderTarget(nullptr);

        EditorEngine::Render();
    }
    
    void OnTick() override {
        EditorEngine::OnTick();
    }
};

Hyperion::Application *Hyperion::CreateApplication() {
    ApplicationSettings settings;
    settings.audio.backend = AudioBackend::None;
    return new SandboxApp(settings);
}