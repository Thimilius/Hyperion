#include <hyperion/hyperion.hpp>
#include <hyperion/entry_point.hpp>

#include <hyperion/editor/editor_engine.hpp>

using namespace Hyperion;
using namespace Hyperion::Rendering;

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

        MeshRenderer *renderer = GetEntity()->GetComponent<MeshRenderer>();
        Ref<Mesh> mesh = renderer->GetSharedMesh();

        if (Input::GetKeyDown(KeyCode::K)) {
            Destroy(GetEntity());
        }
    }
};

class SandboxApp : public Application {
public:
    SandboxApp(const ApplicationSettings &settings) : Application(settings) { }
protected:
    void OnInit() override {
        Entity *entity = Entity::CreatePrimitive(EntityPrimitive::Cube);
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

    void OnUpdate(f32 delta_time) override {
        Ray ray = Editor::EditorEngine::GetCamera()->ScreenPointToRay(Input::GetMousePosition());
        Physics::RaycastResult result;
        if (Physics::PhysicsEngine::Raycast(ray, result)) {
            HYP_TRACE("{}", result.collider->GetEntity()->GetName());
        }
    }
};

Hyperion::Application *Hyperion::CreateApplication() {
    return new SandboxApp(ApplicationSettings::FromJsonFile("app.json"));
}