#include "hyppch.hpp"

#include "hyperion/editor/editor_gizmo.hpp"

#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/assets/mesh_factory.hpp"
#include "hyperion/entity/entity.hpp"
#include "hyperion/entity/world.hpp"
#include "hyperion/entity/components/rendering/mesh_renderer.hpp"
#include "hyperion/entity/components/physics/box_collider.hpp"
#include "hyperion/physics/physics_world.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion::Editor {

    void EditorGizmo::OnCreate() {
        RegisterUpdate();

        Ref<Material> material = Material::Create(AssetManager::GetShader("standard_unlit"));
        Ref<Mesh> mesh = MeshFactory::CreateFromFile("data/models/gizmo.obj");

        m_gimzo_x = Entity::Create("GizmoPartX", Vec3::Zero(), Quaternion::Identity(), GetTransform());
        m_gimzo_x->AddTag("X");
        m_gimzo_x->GetTransform()->SetEulerAngles(Vec3(0.0f, 0.0f, -90.0));
        BoxCollider *collider = m_gimzo_x->AddComponent<BoxCollider>();
        collider->SetOrigin(Vec3(0.0f, 0.5f, 0.0f));
        collider->SetSize(Vec3(0.1f, 1.0f, 0.1f));
        MeshRenderer *renderer = m_gimzo_x->AddComponent<MeshRenderer>();
        renderer->SetMesh(mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", Color::Red());

        m_gimzo_y = Entity::Create("GizmoPartY", Vec3::Zero(), Quaternion::Identity(), GetTransform());
        m_gimzo_y->AddTag("Y");
        collider = m_gimzo_y->AddComponent<BoxCollider>();
        collider->SetOrigin(Vec3(0.0f, 0.5f, 0.0f));
        collider->SetSize(Vec3(0.1f, 1.0f, 0.1f));
        renderer = m_gimzo_y->AddComponent<MeshRenderer>();
        renderer->SetMesh(mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", Color::Green());

        m_gimzo_z = Entity::Create("GizmoPartZ", Vec3::Zero(), Quaternion::Identity(), GetTransform());
        m_gimzo_z->AddTag("Z");
        m_gimzo_z->GetTransform()->SetEulerAngles(Vec3(90.0f, 0.0f, 0.0f));
        collider = m_gimzo_z->AddComponent<BoxCollider>();
        collider->SetOrigin(Vec3(0.0f, 0.5f, 0.0f));
        collider->SetSize(Vec3(0.1f, 1.0f, 0.1f));
        renderer = m_gimzo_z->AddComponent<MeshRenderer>();
        renderer->SetMesh(mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", Color::Blue());
    }

    void EditorGizmo::OnUpdate(f32 delta_time) {
        Transform *transform = GetTransform();
        Vec3 position = transform->GetPosition();

        f32 scale_factor = (m_camera->GetTransform()->GetPosition() - position).Magnitude() * m_camera->GetFOV() * m_gizmo_scale;
        Vec3 scale = Vec3(scale_factor, scale_factor, scale_factor);
        transform->SetLocalScale(scale);

        Ray ray = m_camera->ScreenPointToRay(Input::GetMousePosition());
        Physics::RaycastResult result;
        if (GetWorld()->GetPhysicsWorld()->Raycast(ray, result, 1000)) {
            Entity *entity = result.collider->GetEntity();
            MeshRenderer *renderer = entity->GetComponent<MeshRenderer>();

            if (m_last_entity != nullptr && m_last_entity != entity) {
                ResetColor();
            }
            
            result.collider->GetEntity()->GetComponent<MeshRenderer>()->GetMaterial()->SetColor("u_color", Color::White());

            m_last_entity = entity;
        } else {
            if (m_last_entity != nullptr) {
                ResetColor();
                m_last_entity = nullptr;
            }
        }
    }

    void EditorGizmo::ResetColor() {
        Color color;
        if (m_last_entity->HasTag("X")) {
            color = Color::Red();
        } else if (m_last_entity->HasTag("Y")) {
            color = Color::Green();
        } else if (m_last_entity->HasTag("Z")) {
            color = Color::Blue();
        }
        m_last_entity->GetComponent<MeshRenderer>()->GetMaterial()->SetColor("u_color", color);
    }

}