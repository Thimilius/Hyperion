#include <hyperion/hyperion.hpp>
#include <hyperion/entry_point.hpp>

using namespace Hyperion;
using namespace Hyperion::Rendering;
using namespace Hyperion::Physics;

class Rotator : public Component {
    HYP_OBJECT(Rotator, Component);
private:
    f32 m_rotation_speed = 50.0f;

    void OnCreate() override {
        RegisterUpdate();
    }

    void OnUpdate(f32 delta_time) override {
        Transform *transform = GetTransform();
        Vec3 rotation = transform->GetEulerAngles();
        rotation.y += delta_time * m_rotation_speed;
        transform->SetEulerAngles(rotation);

        if (Input::GetKeyDown(KeyCode::K)) {
            Destroy(GetEntity());
        }
    }
};

class SandboxApp : public Application {
public:
    SandboxApp(const ApplicationSettings &settings) : Application(settings) { }
protected:
    Camera *m_camera;

    void OnInit() override {
        m_camera = WorldManager::GetActiveWorld()->FindComponentOfType<Camera>();

        Entity *entity = Entity::CreatePrimitive(EntityPrimitive::Sphere);
        entity->AddComponent<Rotator>();
        entity->GetComponent<MeshRenderer>()->GetMaterial()->SetTexture2D("u_texture", AssetManager::GetTexture2D("earth"));

        Light *light = Entity::CreatePrimitive(EntityPrimitive::DirectionalLight)->GetComponent<Light>();
        light->SetColor(Color::White());
        light->GetTransform()->SetEulerAngles(Vec3(0, -45.0f, 0));
        
        Light *point_light_0 = Entity::CreatePrimitive(EntityPrimitive::PointLight)->GetComponent<Light>();
        point_light_0->GetTransform()->SetPosition(Vec3(-1, 2, 2));
        point_light_0->SetColor(Color::Red());
        
        Light *point_light_1 = Entity::CreatePrimitive(EntityPrimitive::PointLight)->GetComponent<Light>();
        point_light_1->GetTransform()->SetPosition(Vec3(1, 1, 2));
        point_light_1->SetColor(Color::Green());
        
        Light *point_light_2 = Entity::CreatePrimitive(EntityPrimitive::PointLight)->GetComponent<Light>();
        point_light_2->GetTransform()->SetPosition(Vec3(0, 1, -2));
        point_light_2->SetColor(Color::Blue());
        
        Light *point_light_3 = Entity::CreatePrimitive(EntityPrimitive::PointLight)->GetComponent<Light>();
        point_light_3->GetTransform()->SetPosition(Vec3(0, 1, -1));
        point_light_3->SetColor(Color::Yellow());
    }
};

Hyperion::Application *Hyperion::CreateApplication() {
    return new SandboxApp(ApplicationSettings::FromJsonFile("app.json"));
}