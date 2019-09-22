#include <hyperion/hyperion.hpp>

using namespace Hyperion;
using namespace Hyperion::Events;
using namespace Hyperion::Math;
using namespace Hyperion::Rendering;

class CSandboxApp : public CApplication {
public:
    CSandboxApp() : CApplication(SApplicationSettings()) { }
protected:
    TRef<CShader> m_shader;
    TRef<CTexture2D> m_texture;
    TRef<CMesh> m_mesh;

    TRef<CRenderTexture> m_render_texture;
    TRef<CPerspectiveCamera> m_camera = std::make_shared<CPerspectiveCamera>(); 

    TRef<CFileWatcher> m_watcher;

    void UpdateTitle() {
        TString title = CStringUtils::Format("Hyperion | FPS: {} ({:.2f} ms) | VSync: {}", CTime::GetFPS(), CTime::GetFrameTime(), GetWindow()->GetVSyncMode() != EVSyncMode::DontSync);
        GetWindow()->SetTitle(title);
    }

    void OnInit() override {
        GetWindow()->SetIcon("icon/icon.ico");

        UpdateTitle();

        m_watcher = CFileWatcher::Create("data/shaders/", [this](EFileStatus status, const TString &path) {
            if (CStringUtils::EndsWith(path, "simple.glsl")) {
                m_shader = CShaderLibrary::Reload("simple");
            }
        }, true);

        m_shader = CShaderLibrary::Load("simple", "data/shaders/simple.glsl");
        m_texture = CTexture2D::CreateFromFile("data/textures/grass.png", ETextureWrapMode::Clamp, ETextureFilter::Bilinear, ETextureAnisotropicFilter::None);

        m_mesh = CMesh::CreateCube(1);

        m_render_texture = CRenderTexture::Create(GetWindow()->GetWidth(), GetWindow()->GetHeight(), ERenderTextureFormat::RGBA8);
    }
    
    void OnEvent(CEvent &event) override {
        if (event.GetType() == EEventType::WindowResize) {
            m_render_texture->Resize(GetWindow()->GetWidth(), GetWindow()->GetHeight());
        }
    }

    void OnUpdate(float delta_time) override {
        if (CInput::GetKey(EKeyCode::Control) && CInput::GetKeyDown(EKeyCode::W)) {
            Exit();
        }

        if (CInput::GetKeyDown(EKeyCode::F1)) {
            GetWindow()->SetWindowMode(GetWindow()->GetWindowMode() == EWindowMode::Windowed ? EWindowMode::Borderless : EWindowMode::Windowed);
        }
        if (CInput::GetKeyDown(EKeyCode::F2)) {
            GetWindow()->SetVSyncMode(GetWindow()->GetVSyncMode() == EVSyncMode::DontSync ? EVSyncMode::EveryVBlank : EVSyncMode::DontSync);
            UpdateTitle();
        }

        m_camera->Update();
        m_watcher->Update();
    }

    void OnRender() override {
        CRenderCommand::SetActiveRenderTarget(m_render_texture);

        float clear_color = CMathf::Sin((float)CTime::GetTime()) / 2.0f + 0.5f;
        CRenderCommand::SetClearColor(clear_color, clear_color, clear_color, clear_color);
        CRenderCommand::Clear(EClearMask::Color | EClearMask::Depth);

        CRenderCommand::EnableFeature(EFeature::DepthTesting);

        CRenderCommand::EnableFeature(EFeature::Culling);
        CRenderCommand::SetFrontFaceMode(EFrontFaceMode::Clockwise);
        CRenderCommand::SetCullingMode(ECullingMode::Back);

        CRenderCommand::EnableFeature(EFeature::Blending);
        CRenderCommand::SetBlendFunc(EBlendFactor::SourceAlpha, EBlendFactor::InverseSourceAlpha);

        CRenderer::Begin(*m_camera);
        {
            m_shader->Bind();
            m_shader->SetInt("u_texture", 0);
            m_texture->Bind(0);
            s32 size = 25;
            CRenderer::Submit(m_mesh, m_shader, SMat4::Translate(0, 0, -1));
        }
        CRenderer::End();

        CRenderCommand::Blit(
            nullptr, 
            0,
            0, 
            GetWindow()->GetWidth(), 
            GetWindow()->GetHeight(), 
            m_render_texture, 
            0,
            0, 
            m_render_texture->GetWidth(), 
            m_render_texture->GetHeight()
        );

        // Setting back buffer as render target before swapping buffers fixes vsync
        CRenderCommand::SetActiveRenderTarget(nullptr);
    }
    
    void OnTick() override {
        UpdateTitle();
    }
};

Hyperion::CApplication *Hyperion::CreateApplication() {
    return new CSandboxApp();
}