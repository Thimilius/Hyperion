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

    Entity *m_entity_parent;
    Entity *m_entity_child;

    void OnInit() override {
        EditorLayer *editor_layer = new EditorLayer();
        PushLayer(editor_layer);
        
        m_camera = editor_layer->GetCamera();

        Ref<Mesh> mesh = MeshFactory::CreateCube(1);
        Ref<Material> material = Material::Create(AssetLibrary::GetShader("phong"));
        m_skybox_texture = AssetLibrary::LoadTextureCubemap("skybox", "data/textures/galaxy", ".png");

        material->SetTexture2D("u_texture", AssetLibrary::GetTexture2D("grass"));
        
        Vec3 light_position = Vec3(1.5f, 2.0f, 3.0f);
        Color light_color = Color::White();
        material->SetVec3("u_light.position", light_position);
        material->SetColor("u_light.color", light_color);

        m_entity_parent = Entity::Create("Entity_0_0");
        m_entity_parent->GetTransform()->SetPosition(Vec3(0, 0, 0));
        MeshRendererComponent *mesh_renderer = m_entity_parent->AddComponent<MeshRendererComponent>();
        mesh_renderer->SetMesh(mesh);
        mesh_renderer->SetMaterial(material);

        m_entity_child = Entity::Create(StringUtils::Format("Entity_1_0"));
        m_entity_child->GetTransform()->SetPosition(Vec3(2, 0, 0));
        m_entity_child->GetTransform()->SetLocalScale(Vec3(0.5f, 0.5f, 0.5f));
        m_entity_child->GetTransform()->SetParent(m_entity_parent->GetTransform());
        mesh_renderer = m_entity_child->AddComponent<MeshRendererComponent>();
        mesh_renderer->SetMesh(mesh);
        mesh_renderer->SetMaterial(material);
    }

    void OnUpdate(f32 delta_time) override {
        f32 speed = 50.0f;
        
        Vec3 rotation = m_entity_parent->GetTransform()->GetRotation().ToEulerAngles();
        rotation.y += delta_time * speed;
        m_entity_parent->GetTransform()->SetRotation(Quaternion::FromEulerAngles(rotation));

        rotation = m_entity_child->GetTransform()->GetRotation().ToEulerAngles();
        rotation.y += delta_time * speed;
        m_entity_child->GetTransform()->SetRotation(Quaternion::FromEulerAngles(rotation));
    }

    void OnRender() override {
        RenderEngine::Clear(ClearMask::Color | ClearMask::Depth, Color::Black());

        Renderer::Begin(m_camera);
        {
            Renderer::DrawSkybox(m_skybox_texture);
            RenderEngine::Clear(ClearMask::Depth);
            Renderer::DrawWorld(WorldManager::GetActiveWorld());
        }
        Renderer::End();
    }
};

Hyperion::Application *Hyperion::CreateApplication() {
    ApplicationSettings settings;
    settings.audio.backend = AudioBackend::None;
    return new SandboxApp(settings);
}