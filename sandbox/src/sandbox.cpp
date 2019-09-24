#include <hyperion/hyperion.hpp>

using namespace Hyperion;
using namespace Hyperion::Events;
using namespace Hyperion::Math;
using namespace Hyperion::IO;
using namespace Hyperion::Rendering;
using namespace Hyperion::Audio;
using namespace Hyperion::Entity;

class CSandboxApp : public CApplication {
public:
    CSandboxApp() : CApplication(SApplicationSettings()) { }
protected:
    TRef<CShader> m_lambertian_shader;
    TRef<CTexture2D> m_texture;
    TRef<CMesh> m_mesh;

    TRef<CRenderTexture> m_render_texture;
    TRef<CPerspectiveCameraController> m_camera_controller = std::make_shared<CPerspectiveCameraController>(std::make_shared<CPerspectiveCamera>()); 

    TRef<CFileWatcher> m_watcher;

    void UpdateTitle() {
        TString title = CStringUtils::Format("Hyperion | FPS: {} ({:.2f} ms) | VSync: {}", CTime::GetFPS(), CTime::GetFrameTime(), GetWindow()->GetVSyncMode() != EVSyncMode::DontSync);
        GetWindow()->SetTitle(title);
    }

    void OnInit() override {
        GetWindow()->SetIcon("icon/icon.ico");

        UpdateTitle();

        // Load all shaders into shader library
        for (auto &entry : std::filesystem::directory_iterator("data/shaders/")) {
            auto &path = entry.path();
            if (path.extension() == ".glsl") {
                auto &filename = path.filename().string();
                CShaderLibrary::Load(filename.substr(0, filename.length() - 5), path.string());
            }
        }

        m_lambertian_shader = CShaderLibrary::Get("lambertian");
        m_texture = CTexture2D::CreateFromFile("data/textures/grass.png", ETextureWrapMode::Clamp, ETextureFilter::Bilinear, ETextureAnisotropicFilter::None);
        m_mesh = CMesh::CreateCube(1);
        m_render_texture = CRenderTexture::Create(GetWindow()->GetWidth(), GetWindow()->GetHeight(), ERenderTextureFormat::RGBA8);

        m_watcher = CFileWatcher::Create("data/shaders/", [this](EFileStatus status, const TString &path) {
            if (CStringUtils::EndsWith(path, "lambertian.glsl")) {
                m_lambertian_shader = CShaderLibrary::Reload("lambertian");
            }
        }, true);
    }
    
    void OnEvent(CEvent &event) override {
        if (event.GetType() == EEventType::WindowResize) {
            m_render_texture->Resize(GetWindow()->GetWidth(), GetWindow()->GetHeight());
        }
    }

    void OnUpdate(float delta_time) override {
        if ((CInput::GetKey(EKeyCode::LeftControl) || CInput::GetKey(EKeyCode::RightControl)) && CInput::GetKeyDown(EKeyCode::W)) {
            Exit();
        }

        if (CInput::GetKeyDown(EKeyCode::F1)) {
            GetWindow()->SetWindowMode(GetWindow()->GetWindowMode() == EWindowMode::Windowed ? EWindowMode::Borderless : EWindowMode::Windowed);
        }
        if (CInput::GetKeyDown(EKeyCode::F2)) {
            GetWindow()->SetVSyncMode(GetWindow()->GetVSyncMode() == EVSyncMode::DontSync ? EVSyncMode::EveryVBlank : EVSyncMode::DontSync);
            UpdateTitle();
        }

        m_camera_controller->Update(delta_time);
        m_watcher->Update();
    }

    void OnRender() override {
        CRenderCommand::SetActiveRenderTarget(m_render_texture);

        CRenderCommand::SetClearColor(0, 0, 0, 1);
        CRenderCommand::Clear(EClearMask::Color | EClearMask::Depth);

        CRenderCommand::EnableFeature(EFeature::DepthTesting);

        CRenderCommand::EnableFeature(EFeature::Culling);
        CRenderCommand::SetFrontFaceMode(EFrontFaceMode::Clockwise);
        CRenderCommand::SetCullingMode(ECullingMode::Back);

        CRenderCommand::EnableFeature(EFeature::Blending);
        CRenderCommand::SetBlendFunc(EBlendFactor::SourceAlpha, EBlendFactor::InverseSourceAlpha);

        CRenderer::Begin(m_camera_controller->GetCamera());
        {
            m_lambertian_shader->Bind();
            m_lambertian_shader->SetFloat3("u_light.position", SVec3(2, 2, 2));
            m_lambertian_shader->SetFloat3("u_light.color", SVec3(1, 1, 1));
            m_lambertian_shader->SetInt("u_texture", 0);
            m_texture->Bind(0);
            CRenderer::Submit(m_mesh, m_lambertian_shader, SMat4::Translate(0, 0, -1));
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