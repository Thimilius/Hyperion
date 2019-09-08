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
    TRef<CVertexBuffer> m_vertex_buffer;
    TRef<CIndexBuffer> m_index_buffer;
    TRef<CVertexArray> m_vertex_array;

    TRef<CRenderTexture> m_render_texture;
    TRef<CPerspectiveCamera> m_camera = std::make_shared<CPerspectiveCamera>(); 

    TRef<CFileWatcher> m_watcher;

    void UpdateTitle() {
        TString title = CStringUtils::Format("Hyperion | FPS: {} ({:.2f} ms) | VSync: {}", CTime::GetFPS(), CTime::GetFrameTime(), GetWindow()->GetVSyncMode() != EVSyncMode::DontSync);
        GetWindow()->SetTitle(title);
    }

    void OnInit() override {
        GetWindow()->SetIcon("logo/logo.ico");

        UpdateTitle();

        m_watcher.reset(new CFileWatcher("data/shaders/", [this](EFileStatus status, const TString &path) {
            if (CStringUtils::EndsWith(path, "simple.glsl")) {
                TString source = CFileUtilities::ReadFile("data/shaders/simple.glsl");
                if (!source.empty()) {
                    m_shader = CShader::Create(source);
                }
            }
        }, true));

        m_shader = CShader::Create(CFileUtilities::ReadFile("data/shaders/simple.glsl"));
        m_texture.reset(CTexture2D::CreateFromFile("data/textures/grass.png", ETextureWrapMode::Clamp, ETextureFilter::Bilinear));
        
        float verticies[] = { 
             0.5f,  0.5f, 0.0f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
             0.5f, -0.5f, 0.0f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f,  0.0f, 1.0f,  1.0f, 1.0f, 0.0f, 1.0f,
        };
        m_vertex_buffer.reset(CVertexBuffer::Create((u8*)verticies, sizeof(verticies)));
        CBufferLayout buffer_layout({
            SBufferElement("a_position", EShaderDataType::Float3),
            SBufferElement("a_uv", EShaderDataType::Float2),
            SBufferElement("a_color", EShaderDataType::Float4),
        });
        m_vertex_buffer->SetLayout(buffer_layout);

        u32 indicies[] = {
            0, 1, 2,
            0, 2, 3,
        };
        m_index_buffer.reset(CIndexBuffer::Create(indicies, sizeof(indicies)));

        m_vertex_array.reset(CVertexArray::Create());
        m_vertex_array->AddVertexBuffer(m_vertex_buffer);
        m_vertex_array->SetIndexBuffer(m_index_buffer);

        m_render_texture.reset(CRenderTexture::Create(GetWindow()->GetWidth(), GetWindow()->GetHeight(), ERenderTextureFormat::RGBA8));
    }
    
    void OnEvent(CEvent &event) override {
        if (event.GetType() == EEventType::WindowResize) {
            //m_render_texture->Resize(GetWindow()->GetWidth(), GetWindow()->GetHeight());
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

        //CRenderCommand::EnableFeature(EFeature::Culling);
        CRenderCommand::SetFrontFaceMode(EFrontFaceMode::Clockwise);
        CRenderCommand::SetCullingMode(ECullingMode::Back);

        CRenderCommand::EnableFeature(EFeature::Blending);
        CRenderCommand::SetBlendFunc(EBlendFactor::SourceAlpha, EBlendFactor::InverseSourceAlpha);

        CRenderer::Begin(*m_camera);
        {
            m_shader->Bind();
            m_shader->SetInt("u_texture", 0);
            m_texture->Bind(0);
            CRenderer::Submit(m_shader, m_vertex_array, SMat4::Translate(0, 0, -1));
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