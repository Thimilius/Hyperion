#include <hyperion/hyperion.hpp>
#include <hyperion/entry_point.hpp>

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
    TRef<CShader> m_shader;
    TRef<CTexture2D> m_texture;
    TRef<CMesh> m_mesh;

    TRef<CRenderTexture> m_render_texture;
    TRef<COrthographicCameraController> m_camera_controller = std::make_shared<COrthographicCameraController>(std::make_shared<COrthographicCamera>());

    void UpdateTitle() {
        TString title = CStringUtils::Format("Hyperion | FPS: {} ({:.2f} ms) | VSync: {}", CTime::GetFPS(), CTime::GetFrameTime(), GetWindow()->GetVSyncMode() != EVSyncMode::DontSync);
        GetWindow()->SetTitle(title);
    }

    void OnInit() override {
        GetWindow()->SetIcon("icon/icon.ico");

        UpdateTitle();

        CShaderLibrary::Init("data/shaders/");

        m_shader = CShaderLibrary::Get("phong");
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
        if (CInput::GetKeyDown(EKeyCode::Escape) || ((CInput::GetKey(EKeyCode::LeftControl) || CInput::GetKey(EKeyCode::RightControl)) && CInput::GetKeyDown(EKeyCode::W))) {
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

        CShaderLibrary::Update();
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
            m_shader->Bind();
            m_shader->SetFloat3("u_camera.position", m_camera_controller->GetCamera()->GetPosition());
            m_shader->SetFloat3("u_light.position", SVec3(2, 2, 2));
            m_shader->SetFloat3("u_light.color", SVec3(1, 1, 1));
            m_shader->SetInt("u_texture", 0);
            m_texture->Bind(0);
            CRenderCommand::SetPolygonMode(EPolygonMode::Fill);
            CRenderer::Submit(m_mesh, m_shader, SMat4::Rotate(SVec3(0, 1, 0), (float)CTime::GetTime() * 50.0f));
        }
        CRenderer::End();

        CImmediateRenderer::Begin(m_camera_controller->GetCamera());
        {
            CRenderCommand::SetPolygonMode(EPolygonMode::Line);
            CRenderCommand::DisableFeature(EFeature::Culling);
            //CImmediateRenderer::DrawCube(m_mesh->GetBounds().center, m_mesh->GetBounds().GetSize(), SVec4(1, 1, 1, 1));
        }
        CImmediateRenderer::End();

        CRenderCommand::Blit(nullptr, m_render_texture);

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