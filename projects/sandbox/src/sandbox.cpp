#include <hyperion/hyperion.hpp>
#include <hyperion/entry_point.hpp>

using namespace Hyperion;
using namespace Hyperion::Rendering;
using namespace Hyperion::Audio;
using namespace Hyperion::Editor;

class SandboxApp : public Application {
public:
    SandboxApp() : Application(ApplicationSettings()) { }
protected:
    Ref<RenderTexture> m_render_texture;
    Ref<Camera> m_camera = std::make_shared<Camera>();
    Ref<CameraController> m_camera_controller = std::make_shared<CameraController>(m_camera);

    void UpdateTitle() {
        String title = StringUtils::Format("Hyperion | FPS: {} ({:.2f} ms) | VSync: {}", Time::GetFPS(), Time::GetFrameTime(), GetWindow()->GetVSyncMode() != VSyncMode::DontSync);
        GetWindow()->SetTitle(title);
    }

    void OnInit() override {
        GetWindow()->SetIcon("icon/icon.ico");

        UpdateTitle();

        m_render_texture = RenderTexture::Create(GetWindow()->GetWidth(), GetWindow()->GetHeight(), RenderTextureFormat::RGBA8);
        
        Gizmos::Init();
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
        if (Input::GetKeyDown(KeyCode::F3)) {
            m_camera->SetType(m_camera->GetType() == CameraType::Perspective ? CameraType::Orthographic : CameraType::Perspective);
        }

        m_camera_controller->Update(delta_time);

        Gizmos::Update(m_camera);
    }

    void OnRender() override {
        RenderCommand::SetActiveRenderTarget(m_render_texture);

        RenderCommand::SetClearColor(0, 0, 0, 1);
        RenderCommand::Clear(ClearMask::Color | ClearMask::Depth);

        Gizmos::Render(m_camera);

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