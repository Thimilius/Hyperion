#include "hyperion/editor/editor_gizmo.hpp"

#include <hyperion/core/app/input.hpp>
#include <hyperion/core/app/display.hpp>
#include <hyperion/core/math/plane.hpp>
#include <hyperion/assets/asset_manager.hpp>
#include <hyperion/assets/mesh_factory.hpp>
#include <hyperion/entity/entity.hpp>
#include <hyperion/entity/world.hpp>
#include <hyperion/entity/components/rendering/mesh_renderer.hpp>
#include <hyperion/entity/components/physics/box_collider.hpp>
#include <hyperion/physics/physics_world.hpp>

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
        f32 hit_distance = 0.0f;

        Plane xy_plane = Plane(Vec3::Forward(), position.z);
        Plane xz_plane = Plane(Vec3::Up(), -position.y);
        Plane yz_plane = Plane(Vec3::Right(), -position.x);

        if (Input::GetMouseButtonUp(MouseButtonCode::Left)) {
            m_move_type = MoveType::None;

            m_offset = Vec3::Zero();
        }

        Ray ray = m_camera->ScreenPointToRay(Input::GetMousePosition());
        Physics::RaycastResult result;
        if (GetWorld()->GetPhysicsWorld()->Raycast(ray, result, 1000)) {
            Entity *entity = result.collider->GetEntity();
            MeshRenderer *renderer = entity->GetComponent<MeshRenderer>();

            if (m_last_gizmo != nullptr && m_last_gizmo != entity) {
                ResetColor();
            }
            
            result.collider->GetEntity()->GetComponent<MeshRenderer>()->GetMaterial()->SetColor("u_color", Color::White());

            m_last_gizmo = entity;

            if (Input::GetMouseButtonDown(MouseButtonCode::Left)) {
                if (entity->HasTag("X")) {
                    m_move_type = MoveType::XAxis;
                } else if (entity->HasTag("Y")) {
                    m_move_type = MoveType::YAxis;
                } else if (entity->HasTag("Z")) {
                    m_move_type = MoveType::ZAxis;
                }

                switch (m_move_type) {
                    case MoveType::XAxis: {
                        xz_plane.Intersects(ray, hit_distance);
                        m_offset.x = position.x - ray.GetPoint(hit_distance).x;
                        break;
                    }
                    case MoveType::YAxis: {
                        xy_plane.Intersects(ray, hit_distance);
                        m_offset.y = position.y - ray.GetPoint(hit_distance).y;
                        break;
                    }
                    case MoveType::ZAxis: {
                        xz_plane.Intersects(ray, hit_distance);
                        m_offset.z = position.z - ray.GetPoint(hit_distance).z;
                        break;
                    }
                }
            }
        } else {
            if (m_last_gizmo != nullptr) {
                ResetColor();
                m_last_gizmo = nullptr;
            }
        }

        bool snapping = Input::GetKey(KeyCode::LeftControl);
        switch (m_move_type) {
            case MoveType::XAxis: {
                xz_plane.Intersects(ray, hit_distance);
                f32 x = ray.GetPoint(hit_distance).x + m_offset.x;
                if (snapping) {
                    x = Math::Round(x);
                }
                position.x = x;
                break;
            }
            case MoveType::YAxis: {
                xy_plane.Intersects(ray, hit_distance);
                f32 y = ray.GetPoint(hit_distance).y + m_offset.y;
                if (snapping) {
                    y = Math::Round(y);
                }
                position.y = y;
                break;
            }
            case MoveType::ZAxis: {
                xz_plane.Intersects(ray, hit_distance);
                f32 z = ray.GetPoint(hit_distance).z + m_offset.z;
                if (snapping) {
                    z = Math::Round(z);
                }
                position.z = z;
                break;
            }
        }

        GetTransform()->SetPosition(position);
    }

    void EditorGizmo::ResetColor() {
        Color color;
        if (m_last_gizmo->HasTag("X")) {
            color = Color::Red();
        } else if (m_last_gizmo->HasTag("Y")) {
            color = Color::Green();
        } else if (m_last_gizmo->HasTag("Z")) {
            color = Color::Blue();
        }
        m_last_gizmo->GetComponent<MeshRenderer>()->GetMaterial()->SetColor("u_color", color);
    }

}