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

        Light light;
        light.type = LightType::Point;
        light.color = Color::White();
        light.intensity = 1;
        light.range = 50;

        material->SetColor("u_light.color", light.color);
        material->SetFloat("u_light.radius", light.range);
        material->SetFloat("u_light.intensity", light.intensity);
        material->SetVec3("u_light.position", Vec3(1, 2, 2));

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