#include <hyperion/hyperion.hpp>
#include <hyperion/entry_point.hpp>

using namespace Hyperion;

class Rotator : public Component {
    HYP_OBJECT(Rotator, Component);
private:
    f32 m_rotation_speed = 50.0f;

    void OnCreate() override {
        RegisterUpdate();
    }

    void OnUpdate(f32 delta_time) override {
        f32 angle = static_cast<f32>(Time::GetTime()) * m_rotation_speed;
        GetTransform()->SetLocalRotation(Quaternion::FromAxisAngle(Vec3::Up(), angle));

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

        Entity::CreatePrimitive(EntityPrimitive::DirectionalLight, Vec3(), Quaternion::FromEulerAngles(Vec3(0, -45.0f, 0)))->GetComponent<Light>()->SetColor(Color::White());
        Entity::CreatePrimitive(EntityPrimitive::PointLight, Vec3(-1, 2, 2))->GetComponent<Light>()->SetColor(Color::Red());
        Entity::CreatePrimitive(EntityPrimitive::PointLight, Vec3(1, 1, 2))->GetComponent<Light>()->SetColor(Color::Green());
        Entity::CreatePrimitive(EntityPrimitive::PointLight, Vec3(0, 1, -2))->GetComponent<Light>()->SetColor(Color::Blue());
        Entity::CreatePrimitive(EntityPrimitive::PointLight, Vec3(0, 1, -1))->GetComponent<Light>()->SetColor(Color::Yellow());
    }
};

void Hyperion::RegisterApplicationTypes() {

}

Hyperion::Application *Hyperion::CreateApplication() {
    return new SandboxApp(ApplicationSettings::FromJsonFile("app.json"));
}