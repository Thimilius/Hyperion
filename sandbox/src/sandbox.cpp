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
    CPerspectiveCamera *m_camera = new CPerspectiveCamera(); 

    void UpdateTitle() {
        TString title = CStringUtils::Format("Hyperion | FPS: {} ({:.2f} ms) | Vsync: {}", CTime::GetFPS(), CTime::GetFrameTime(), GetWindow()->GetVSyncMode() != EVSyncMode::DontSync);
        GetWindow()->SetTitle(title);
    }

    void OnInit() override {
        GetWindow()->SetIcon("logo/logo.ico");

        UpdateTitle();

        TString vertex_source = R"(
            #version 410 core

            layout(location = 0) in vec3 a_position;
            layout(location = 1) in vec2 a_uv;
            layout(location = 2) in vec4 a_color;

            out VS_OUT {
                vec2 uv;
                vec4 color;
            } vs_out;

            uniform struct Transform {
                mat4 model;
                mat4 view;
                mat4 projection;
            } u_transform;

            vec4 model_to_clip_space(vec3 position) {
                return u_transform.projection * u_transform.view * u_transform.model * vec4(position, 1.0f);
            }

            void main() {
                vs_out.uv = a_uv;
                vs_out.color = a_color;

                gl_Position = model_to_clip_space(a_position);
            }
        )";
        TString fragment_source = R"(
            #version 410 core

            out vec4 o_color;

            in VS_OUT {
                vec2 uv;
                vec4 color;
            } fs_in;

            uniform sampler2D u_texture;

            void main() {
                o_color = texture(u_texture, fs_in.uv);
            }
        )";
        m_shader.reset(CShader::Create(vertex_source, fragment_source));

        m_texture.reset(CTexture2D::CreateFromFile("data/textures/grass.png", ETextureWrapMode::Clamp, ETextureFilter::Bilinear));
        //m_texture.reset(CTexture2D::CreateFromFile("logo/logo.png", ETextureWrapMode::Clamp, ETextureFilter::Bilinear));
        
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
    }
    
    void OnEvent(CEvent &event) override {

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

        if (CInput::GetKey(EKeyCode::Shift)) {
            if (CInput::GetMouseButton(EMouseButtonCode::Middle)) {
                // FIXME: Why does this not work!?!?!
                HYP_TRACE("Middle");
            }
        }

        HYP_TRACE("{}", CInput::GetMouseButton(EMouseButtonCode::Middle));

        m_camera->Update();
    }

    void OnRender() override {
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
    }
    
    void OnTick() override {
        UpdateTitle();
    }
};

Hyperion::CApplication *Hyperion::CreateApplication() {
    return new CSandboxApp();
}