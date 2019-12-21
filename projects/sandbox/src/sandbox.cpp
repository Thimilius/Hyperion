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
    Ref<Shader> m_gizmo_shader;
    Ref<Mesh> m_gizmo_mesh;

    Ref<RenderTexture> m_render_texture;
    Ref<PerspectiveCameraController> m_camera_controller = std::make_shared<PerspectiveCameraController>(std::make_shared<PerspectiveCamera>());

    Bounds m_gizmo_x_bounds;
    bool m_gizmo_x_hit = false;
    Bounds m_gizmo_y_bounds;
    bool m_gizmo_y_hit = false;
    Bounds m_gizmo_z_bounds;
    bool m_gizmo_z_hit = false;

    void UpdateTitle() {
        String title = StringUtils::Format("Hyperion | FPS: {} ({:.2f} ms) | VSync: {}", Time::GetFPS(), Time::GetFrameTime(), GetWindow()->GetVSyncMode() != VSyncMode::DontSync);
        GetWindow()->SetTitle(title);
    }

    void OnInit() override {
        GetWindow()->SetIcon("icon/icon.ico");

        UpdateTitle();

        m_gizmo_shader = ShaderLibrary::Get("gizmo");
        m_gizmo_mesh = Mesh::CreateFromFile("data/models/gizmo.obj");
        m_render_texture = RenderTexture::Create(GetWindow()->GetWidth(), GetWindow()->GetHeight(), RenderTextureFormat::RGBA8);
        m_gizmo_x_bounds = Bounds(Vec3(0.5f, 0.0f, 0.0f), Vec3(1.0f, 0.1f, 0.1f));
        m_gizmo_y_bounds = Bounds(Vec3(0.0f, 0.5f, 0.0f), Vec3(0.1f, 1.0f, 0.1f));
        m_gizmo_z_bounds = Bounds(Vec3(0.0f, 0.0f, 0.5f), Vec3(0.1f, 0.1f, 1.0f));
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

        // Gizmo intersections
        {
            Vec2 mouse_position = Input::GetMousePosition();
            Ray ray = m_camera_controller->GetCamera()->ScreenPointToRay(mouse_position);
            m_gizmo_x_hit = m_gizmo_x_bounds.Intersects(ray);
            m_gizmo_y_hit = m_gizmo_y_bounds.Intersects(ray);
            m_gizmo_z_hit = m_gizmo_z_bounds.Intersects(ray);
        }
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

        RenderCommand::SetPolygonMode(PolygonMode::Fill);

        Renderer::Begin(m_camera_controller->GetCamera());
        {
            // Draw gizmo
            {
                // We draw the gizmos on top of everything so erase depth information
                RenderCommand::Clear(ClearMask::Depth);

                m_gizmo_shader->Bind();
                m_gizmo_shader->SetFloat3("u_camera.position", m_camera_controller->GetCamera()->GetPosition());

                // X gizmo
                m_gizmo_shader->SetFloat3("u_color", m_gizmo_x_hit ? Vec3(1, 1, 1) : Vec3(1, 0, 0));
                Renderer::Submit(m_gizmo_mesh, m_gizmo_shader, Mat4::Rotate(Vec3(0, 0, 1), -90.0f));
                // Y gizmo
                m_gizmo_shader->SetFloat3("u_color", m_gizmo_y_hit ? Vec3(1, 1, 1) : Vec3(0, 1, 0));
                Renderer::Submit(m_gizmo_mesh, m_gizmo_shader, Mat4::Identity());
                // Z gizmo
                m_gizmo_shader->SetFloat3("u_color", m_gizmo_z_hit ? Vec3(1, 1, 1) : Vec3(0, 0, 1));
                Renderer::Submit(m_gizmo_mesh, m_gizmo_shader, Mat4::Rotate(Vec3(1, 0, 0), 90.0f));
            }
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