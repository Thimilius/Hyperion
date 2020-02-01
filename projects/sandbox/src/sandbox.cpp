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

    Ref<TextureCubemap> m_skybox_texture;

    Entity *m_entity;

    void OnInit() override {
        EditorLayer *editor_layer = new EditorLayer();
        PushLayer(editor_layer);
        
        m_camera = editor_layer->GetCamera();

        Ref<Mesh> mesh = MeshFactory::CreateFromFile("data/models/sphere.obj");
        Ref<Material> material = Material::Create(AssetLibrary::GetShader("standard_phong"));
        m_skybox_texture = AssetLibrary::LoadTextureCubemap("skybox", "data/textures/galaxy", ".png");

        material->SetTexture2D("u_texture", AssetLibrary::GetTexture2D("earth"));

        Vec3 light_position = Vec3(1.5f, 2.0f, 3.0f);
        Color light_color = Color::White();
        material->SetVec3("u_light.position", light_position);
        material->SetColor("u_light.color", light_color);

        m_entity = Entity::Create("Entity_0_0", Vec3(0, 0, 0));
        MeshRendererComponent *mesh_renderer = m_entity->AddComponent<MeshRendererComponent>();
        mesh_renderer->SetMesh(mesh);
        mesh_renderer->SetMaterial(material);
    }

    void OnUpdate(f32 delta_time) override {
        f32 speed = 5.0f;
        
        Vec3 rotation = m_entity->GetTransform()->GetEulerAngles();
        rotation.y += delta_time * speed;
        m_entity->GetTransform()->SetEulerAngles(rotation);

        if (Input::GetKeyDown(KeyCode::F)) {
            bool enabled = m_entity->GetComponent<MeshRendererComponent>()->IsEnabled();
            m_entity->GetComponent<MeshRendererComponent>()->SetEnabled(!enabled);
        }
    }

    void OnRender() override {
        RenderEngine::Clear(ClearMask::Color | ClearMask::Depth, Color::Black());

        Renderer::Begin(m_camera);
        {
            Renderer::DrawWorld(WorldManager::GetActiveWorld());
            Renderer::DrawSkybox(m_skybox_texture);
        }
        Renderer::End();
    }
};

Hyperion::Application *Hyperion::CreateApplication() {
    ApplicationSettings settings;
    settings.audio.backend = AudioBackend::None;
    return new SandboxApp(settings);
}