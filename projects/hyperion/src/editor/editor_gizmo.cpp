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
        BoxCollider *collider = m_gimzo_x->AddComponent<BoxCollider>();
        collider->SetOrigin(Vec3(0.5f, 0.0f, 0.0f));
        collider->SetSize(Vec3(1.0f, 0.1f, 0.1f));
        Entity *child = Entity::Create("", Vec3::Zero(), Quaternion::FromEulerAngles(Vec3(0, 0, -90.0f)), m_gimzo_x->GetTransform());
        MeshRenderer *renderer = child->AddComponent<MeshRenderer>();
        renderer->SetMesh(mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", Color::Red());

        m_gimzo_y = Entity::Create("GizmoPartY", Vec3::Zero(), Quaternion::Identity(), GetTransform());
        collider = m_gimzo_y->AddComponent<BoxCollider>();
        collider->SetOrigin(Vec3(0.0f, 0.5f, 0.0f));
        collider->SetSize(Vec3(0.1f, 1.0f, 0.1f));
        child = Entity::Create("", Vec3::Zero(), Quaternion::FromEulerAngles(Vec3(0, 0, 0)), m_gimzo_y->GetTransform());
        renderer = child->AddComponent<MeshRenderer>();
        renderer->SetMesh(mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", Color::Green());

        m_gimzo_z = Entity::Create("GizmoPartZ", Vec3::Zero(), Quaternion::Identity(), GetTransform());
        collider = m_gimzo_z->AddComponent<BoxCollider>();
        collider->SetOrigin(Vec3(0.0f, 0.0f, 0.5f));
        collider->SetSize(Vec3(0.1f, 0.1f, 1.0f));
        child = Entity::Create("", Vec3::Zero(), Quaternion::FromEulerAngles(Vec3(90.0f, 0, 0)), m_gimzo_z->GetTransform());
        renderer = child->AddComponent<MeshRenderer>();
        renderer->SetMesh(mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", Color::Blue());
    }

    void EditorGizmo::OnUpdate(f32 delta_time) {
        f32 scale_factor = m_camera->GetTransform()->GetPosition().Magnitude() * m_camera->GetFOV() * m_gizmo_scale;
        Vec3 scale = Vec3(scale_factor, scale_factor, scale_factor);

        Ray ray = m_camera->ScreenPointToRay(Input::GetMousePosition());
        Physics::RaycastResult result;
        if (GetWorld()->GetPhysicsWorld()->Raycast(ray, result, 1000)) {
            HYP_TRACE("{}", result.collider->GetEntity()->GetName());
        }
    }

}