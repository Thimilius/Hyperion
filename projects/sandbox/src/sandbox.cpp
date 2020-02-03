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
    Entity *m_entity;

    void OnInit() override {
        m_camera = GetEditorLayer()->GetCamera();

        WorldManager::GetActiveWorld()->GetEnvironment().SetSkybox(AssetLibrary::LoadTextureCubemap("skybox", "data/textures/galaxy", ".png"));

        m_entity = Entity::CreatePrimitive(EntityPrimitive::Sphere);
        Ref<Material> material = m_entity->GetComponent<MeshRendererComponent>()->GetSharedMaterial();
        material->SetTexture2D("u_texture", AssetLibrary::GetTexture2D("earth"));

        Light light = Entity::CreatePrimitive(EntityPrimitive::Light)->GetComponent<LightComponent>()->GetLight();
        material->SetColor("u_light.color", light.color);
        material->SetFloat("u_light.radius", light.range);
        material->SetFloat("u_light.intensity", light.intensity);
        material->SetVec3("u_light.position", Vec3(1, 2, 2));
    }

    void OnUpdate(f32 delta_time) override {
        f32 speed = 50.0f;
        
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
        }
        Renderer::End();
    }
};

Hyperion::Application *Hyperion::CreateApplication() {
    ApplicationSettings settings;
    settings.audio.backend = AudioBackend::None;
    return new SandboxApp(settings);
}