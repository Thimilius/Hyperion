#include <hyperion/hyperion.hpp>
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
    Ref<Shader> m_cube_shader;
    Ref<Texture2D> m_cube_texture;
    Ref<TextureCubemap> m_skybox_texture;

    void OnInit() override {
        m_camera = Camera::Create();
        m_render_texture = RenderTexture::Create(GetWindow()->GetWidth(), GetWindow()->GetHeight(), RenderTextureFormat::RGBA8);

        m_cube_mesh = MeshFactory::CreateCube(1);
        m_cube_shader = AssetLibrary::GetShader("phong");
        m_cube_texture = AssetLibrary::GetTexture2D("grass");
        
        m_skybox_texture = AssetLibrary::LoadTextureCubemap("skybox", {
            { TextureCubemapFace::PositiveX, "data/textures/galaxy/galaxy_right.png" },
            { TextureCubemapFace::NegativeX, "data/textures/galaxy/galaxy_left.png" },
            { TextureCubemapFace::PositiveY, "data/textures/galaxy/galaxy_top.png" },
            { TextureCubemapFace::NegativeY, "data/textures/galaxy/galaxy_bottom.png" },
            { TextureCubemapFace::PositiveZ, "data/textures/galaxy/galaxy_back.png" },
            { TextureCubemapFace::NegativeZ, "data/textures/galaxy/galaxy_front.png" },
        });

        EditorEngine::OnInit(m_camera);
    }
    
    void OnEvent(Event &event) override {
        if (event.GetType() == EventType::WindowResize) {
            m_render_texture->Resize(GetWindow()->GetWidth(), GetWindow()->GetHeight());
        }
    }

    void OnUpdate(f32 delta_time) override {
        EditorEngine::OnUpdate(m_camera);
    }

    void OnRender() override {
        RenderEngine::SetActiveRenderTarget(m_render_texture);
        
        RenderEngine::SetClearColor(0, 0, 0, 1);
        RenderEngine::Clear(ClearMask::Color | ClearMask::Depth);

        Renderer::Begin(m_camera);
        {
            Renderer::DrawSkybox(m_skybox_texture);

            RenderEngine::Clear(ClearMask::Depth);

            m_cube_shader->Bind();
            m_cube_shader->SetFloat3("u_camera.position", m_camera->GetPosition());
            m_cube_shader->SetFloat3("u_light.position", Vec3(1.5f, 2.0f, 3.0f));
            m_cube_shader->SetFloat3("u_light.color", Vec3::One());
            m_cube_shader->SetInt("u_texture", 0);
            m_cube_texture->Bind(0);
            Renderer::Draw(m_cube_mesh, m_cube_shader, Mat4::Identity());
        }
        Renderer::End();

        RenderEngine::Blit(nullptr, m_render_texture);
        // Setting back buffer as render target before swapping buffers fixes vsync
        RenderEngine::SetActiveRenderTarget(nullptr);

        EditorEngine::OnRender(m_camera);
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