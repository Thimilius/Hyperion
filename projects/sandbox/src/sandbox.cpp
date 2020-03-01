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
    Entity *m_entity;

    void OnInit() override {
        WorldManager::GetActiveWorld()->GetEnvironment().background_skybox = AssetLibrary::LoadTextureCubemap("skybox", "data/textures/galaxy", ".png");

        m_entity = Entity::CreatePrimitive(EntityPrimitive::Sphere);
        Ref<Material> material = m_entity->GetComponent<MeshRenderer>()->GetSharedMaterial();
        material->SetTexture2D("u_texture", AssetLibrary::GetTexture2D("earth"));

        Light *light = Entity::CreatePrimitive(EntityPrimitive::Light)->GetComponent<Light>();
        light->GetTransform()->SetPosition(Vec3(-1, 2, 2));
        light->SetColor(Color::White());
    }

    void OnUpdate(f32 delta_time) override {
        f32 speed = 50.0f;
        
        Vec3 rotation = m_entity->GetTransform()->GetEulerAngles();
        rotation.y += delta_time * speed;
        m_entity->GetTransform()->SetEulerAngles(rotation);

        if (Input::GetKeyDown(KeyCode::F)) {
            bool enabled = m_entity->GetComponent<MeshRenderer>()->IsEnabled();
            m_entity->GetComponent<MeshRenderer>()->SetEnabled(!enabled);
        }
    }
};

Hyperion::Application *Hyperion::CreateApplication() {
    ApplicationSettings settings;
    settings.audio.backend = AudioBackend::None;
    return new SandboxApp(settings);
}