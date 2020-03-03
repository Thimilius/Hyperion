#include <hyperion/hyperion.hpp>
#include <hyperion/entry_point.hpp>

using namespace Hyperion;
using namespace Hyperion::Rendering;

class SandboxApp : public Application {
public:
    SandboxApp(const ApplicationSettings &settings) : Application(settings) { }
protected:
    Entity *m_entity;
    Light *m_light;

    void OnInit() override {
        m_entity = Entity::CreatePrimitive(EntityPrimitive::Sphere);
        Ref<Material> material = m_entity->GetComponent<MeshRenderer>()->GetSharedMaterial();
        material->SetTexture2D("u_texture", AssetManager::GetTexture2D("earth"));

        m_light = Entity::CreatePrimitive(EntityPrimitive::DirectionalLight)->GetComponent<Light>();
        m_light->SetColor(Color::White());
        m_light->GetTransform()->SetEulerAngles(Vec3(0, -45.0f, 0));
        
        Light *point_light1 = Entity::CreatePrimitive(EntityPrimitive::PointLight)->GetComponent<Light>();
        point_light1->GetTransform()->SetPosition(Vec3(-1, 2, 2));
        point_light1->SetColor(Color::Red());
        
        Light *point_light2 = Entity::CreatePrimitive(EntityPrimitive::PointLight)->GetComponent<Light>();
        point_light2->GetTransform()->SetPosition(Vec3(1, 1, 2));
        point_light2->SetColor(Color::Green());
        
        Light *point_light3 = Entity::CreatePrimitive(EntityPrimitive::PointLight)->GetComponent<Light>();
        point_light3->GetTransform()->SetPosition(Vec3(0, 1, -2));
        point_light3->SetColor(Color::Blue());
        
        Light *point_light4 = Entity::CreatePrimitive(EntityPrimitive::PointLight)->GetComponent<Light>();
        point_light4->GetTransform()->SetPosition(Vec3(0, 1, -1));
        point_light4->SetColor(Color::Yellow());
    }

    void OnUpdate(f32 delta_time) override {
        f32 speed = 50.0f;
        
        Vec3 rotation = m_entity->GetTransform()->GetEulerAngles();
        rotation.y += delta_time * speed;
        m_entity->GetTransform()->SetEulerAngles(rotation);

        if (Input::GetKeyDown(KeyCode::L)) {
            m_light->SetEnabled(!m_light->IsEnabled());
        }
    }
};

Hyperion::Application *Hyperion::CreateApplication() {
    return new SandboxApp(ApplicationSettings::FromJsonFile("app.json"));
}