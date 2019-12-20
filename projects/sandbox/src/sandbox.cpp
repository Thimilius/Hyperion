#include <hyperion/hyperion.hpp>
#include <hyperion/entry_point.hpp>

using namespace Hyperion;
using namespace Hyperion::Rendering;
using namespace Hyperion::Audio;
using namespace Hyperion::Entity;

class SandboxApp : public Application {
public:
    SandboxApp() : Application(ApplicationSettings()) { }
protected:
    Ref<Shader> m_shader;
    Ref<Texture2D> m_texture;
    Ref<Mesh> m_mesh;

    Ref<RenderTexture> m_render_texture;
    Ref<PerspectiveCameraController> m_camera_controller = std::make_shared<PerspectiveCameraController>(std::make_shared<PerspectiveCamera>());

    void UpdateTitle() {
        String title = StringUtils::Format("Hyperion | FPS: {} ({:.2f} ms) | VSync: {}", Time::GetFPS(), Time::GetFrameTime(), GetWindow()->GetVSyncMode() != VSyncMode::DontSync);
        GetWindow()->SetTitle(title);
    }

    void OnInit() override {
        GetWindow()->SetIcon("icon/icon.ico");

        UpdateTitle();

        ShaderLibrary::Init("data/shaders/");

        m_shader = ShaderLibrary::Get("phong");
        m_texture = Texture2D::CreateFromFile("data/textures/grass.png", TextureWrapMode::Clamp, TextureFilter::Bilinear, TextureAnisotropicFilter::None);
        m_mesh = Mesh::CreateCube(1);
        m_render_texture = RenderTexture::Create(GetWindow()->GetWidth(), GetWindow()->GetHeight(), RenderTextureFormat::RGBA8);
    }
    
    void OnEvent(Event &event) override {
        if (event.GetType() == EventType::WindowResize) {
            m_render_texture->Resize(GetWindow()->GetWidth(), GetWindow()->GetHeight());
        }
    }

    void OnUpdate(f32 delta_time) override {
        if (Input::GetKeyDown(KeyCode::Escape) || ((Input::GetKey(KeyCode::LeftControl) || Input::GetKey(KeyCode::RightControl)) && Input::GetKeyDown(KeyCode::W))) {
            Exit();
        }

        if (Input::GetKeyDown(KeyCode::F1)) {
            GetWindow()->SetWindowMode(GetWindow()->GetWindowMode() == WindowMode::Windowed ? WindowMode::Borderless : WindowMode::Windowed);
        }
        if (Input::GetKeyDown(KeyCode::F2)) {
            GetWindow()->SetVSyncMode(GetWindow()->GetVSyncMode() == VSyncMode::DontSync ? VSyncMode::EveryVBlank : VSyncMode::DontSync);
            UpdateTitle();
        }

        m_camera_controller->Update(delta_time);

        ShaderLibrary::Update();
    }

    void OnRender() override {
        RenderCommand::SetActiveRenderTarget(m_render_texture);

        RenderCommand::SetClearColor(0, 0, 0, 1);
        RenderCommand::Clear(ClearMask::Color | ClearMask::Depth);

        RenderCommand::EnableFeature(Feature::DepthTesting);

        RenderCommand::EnableFeature(Feature::Culling);
        RenderCommand::SetFrontFaceMode(FrontFaceMode::Clockwise);
        RenderCommand::SetCullingMode(CullingMode::Back);

        RenderCommand::EnableFeature(Feature::Blending);
        RenderCommand::SetBlendFunc(BlendFactor::SourceAlpha, BlendFactor::InverseSourceAlpha);

        Renderer::Begin(m_camera_controller->GetCamera());
        {
            m_shader->Bind();
            m_shader->SetFloat3("u_camera.position", m_camera_controller->GetCamera()->GetPosition());
            m_shader->SetFloat3("u_light.position", Vec3(2, 2, 2));
            m_shader->SetFloat3("u_light.color", Vec3(1, 1, 1));
            m_shader->SetInt("u_texture", 0);
            m_texture->Bind(0);
            RenderCommand::SetPolygonMode(PolygonMode::Fill);
            Renderer::Submit(m_mesh, m_shader, Mat4::Rotate(Vec3(0, 1, 0), Time::GetTime() * 50.0f));
        }
        Renderer::End();

        RenderCommand::Blit(nullptr, m_render_texture);

        // Setting back buffer as render target before swapping buffers fixes vsync
        RenderCommand::SetActiveRenderTarget(nullptr);
    }
    
    void OnTick() override {
        UpdateTitle();
    }
};

Hyperion::Application *Hyperion::CreateApplication() {
    return new SandboxApp();
}