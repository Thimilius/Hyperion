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
#include <hyperion/entity/components/physics/sphere_collider.hpp>
#include <hyperion/physics/physics_world.hpp>

using namespace Hyperion::Rendering;

namespace Hyperion::Editor {

    void EditorGizmo::SetGizmoType(GizmoType type) {
        m_gizmo_type = type;

        m_gizmo_position_axis->SetActive(false);
        m_gizmo_position_plane->SetActive(false);
        m_gizmo_position_full->SetActive(false);
        m_gizmo_rotation->SetActive(false);
        m_gizmo_scale->SetActive(false);
        m_gizmo_scale_full->SetActive(false);

        switch (type) {
            case EditorGizmo::GizmoType::Position:
                m_gizmo_position_axis->SetActive(true);
                m_gizmo_position_plane->SetActive(true);
                m_gizmo_position_full->SetActive(true);
                break;
            case EditorGizmo::GizmoType::Rotation:
                m_gizmo_rotation->SetActive(true);
                break;
            case EditorGizmo::GizmoType::Scale:
                m_gizmo_scale->SetActive(true);
                m_gizmo_scale_full->SetActive(true);
                break;
            default: HYP_ASSERT_ENUM_OUT_OF_RANGE; break;
        }
    }

    void EditorGizmo::OnCreate() {
        RegisterForUpdate();

        Material *material = Material::Create(AssetManager::GetShader("standard_unlit"));
        Mesh *position_mesh = MeshFactory::CreateFromFile("data/models/gizmo/gizmo_position.obj");
        Mesh *position_plane_mesh = AssetManager::GetMeshPrimitive(MeshPrimitive::Quad);
        Mesh *rotation_mesh = MeshFactory::CreateFromFile("data/models/gizmo/gizmo_rotation.obj");
        Mesh *scale_mesh = MeshFactory::CreateFromFile("data/models/gizmo/gizmo_scale.obj");

        m_gizmo_position_axis = Entity::Create("Gizmo_Position_Axis", Vec3::Zero(), Quaternion::Identity(), GetTransform());
        m_gizmo_position_plane = Entity::Create("Gizmo_Position_Plane", Vec3::Zero(), Quaternion::Identity(), GetTransform());
        m_gizmo_position_full = Entity::Create("Gizmo_Position_Full", Vec3::Zero(), Quaternion::Identity(), GetTransform());
        m_gizmo_rotation = Entity::Create("Gizmo_Rotation", Vec3::Zero(), Quaternion::Identity(), GetTransform());
        m_gizmo_scale = Entity::Create("Gizmo_Scale", Vec3::Zero(), Quaternion::Identity(), GetTransform());
        m_gizmo_scale_full = Entity::Create("Gizmo_Scale_Full", Vec3::Zero(), Quaternion::Identity(), GetTransform());

        Entity *entity = Entity::Create("Gizmo_Position_X", Vec3::Zero(), Quaternion::Identity(), m_gizmo_position_axis->GetTransform());
        entity->AddTag(POSITION_X_TAG);
        entity->GetTransform()->SetEulerAngles(Vec3(0.0f, 0.0f, -90.0));
        BoxCollider *collider = entity->AddComponent<BoxCollider>();
        collider->SetOrigin(Vec3(0.0f, 0.5f, 0.0f));
        collider->SetSize(Vec3(0.1f, 1.0f, 0.1f));
        MeshRenderer *renderer = entity->AddComponent<MeshRenderer>();
        renderer->SetMesh(position_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_x_axis_color);

        entity = Entity::Create("Gizmo_Position_Y", Vec3::Zero(), Quaternion::Identity(), m_gizmo_position_axis->GetTransform());
        entity->AddTag(POSITION_Y_TAG);
        collider = entity->AddComponent<BoxCollider>();
        collider->SetOrigin(Vec3(0.0f, 0.5f, 0.0f));
        collider->SetSize(Vec3(0.1f, 1.0f, 0.1f));
        renderer = entity->AddComponent<MeshRenderer>();
        renderer->SetMesh(position_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_y_axis_color);
        
        entity = Entity::Create("Gizmo_Position_Z", Vec3::Zero(), Quaternion::Identity(), m_gizmo_position_axis->GetTransform());
        entity->AddTag(POSITION_Z_TAG);
        entity->GetTransform()->SetEulerAngles(Vec3(90.0f, 0.0f, 0.0f));
        collider = entity->AddComponent<BoxCollider>();
        collider->SetOrigin(Vec3(0.0f, 0.5f, 0.0f));
        collider->SetSize(Vec3(0.1f, 1.0f, 0.1f));
        renderer = entity->AddComponent<MeshRenderer>();
        renderer->SetMesh(position_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_z_axis_color);

        entity = Entity::Create("Gizmo_Position_XY", Vec3::Zero(), Quaternion::Identity(), m_gizmo_position_plane->GetTransform());
        entity->AddTag(POSITION_XY_TAG);
        entity->GetTransform()->SetEulerAngles(Vec3(0.0f, 0.0f, 0.0f));
        entity->GetTransform()->SetPosition(Vec3(0.5f, 0.5f, 0.0f));
        entity->GetTransform()->SetLocalScale(Vec3(0.25f, 0.25f, 0.25f));
        collider = entity->AddComponent<BoxCollider>();
        collider->SetSize(Vec3(1.0f, 1.0f, 0.01f));
        renderer = entity->AddComponent<MeshRenderer>();
        renderer->SetMesh(position_plane_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_z_axis_color);

        entity = Entity::Create("Gizmo_Position_XZ", Vec3::Zero(), Quaternion::Identity(), m_gizmo_position_plane->GetTransform());
        entity->AddTag(POSITION_XZ_TAG);
        entity->GetTransform()->SetEulerAngles(Vec3(90.0f, 0.0f, 0.0f));
        entity->GetTransform()->SetPosition(Vec3(0.5f, 0.0f, 0.5f));
        entity->GetTransform()->SetLocalScale(Vec3(0.25f, 0.25f, 0.25f));
        collider = entity->AddComponent<BoxCollider>();
        collider->SetSize(Vec3(1.0f, 1.0f, 0.01f));
        renderer = entity->AddComponent<MeshRenderer>();
        renderer->SetMesh(position_plane_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_y_axis_color);

        entity = Entity::Create("Gizmo_Position_YZ", Vec3::Zero(), Quaternion::Identity(), m_gizmo_position_plane->GetTransform());
        entity->AddTag(POSITION_YZ_TAG);
        entity->GetTransform()->SetEulerAngles(Vec3(0.0f, 90.0f, 0.0f));
        entity->GetTransform()->SetPosition(Vec3(0.0f, 0.5f, 0.5f));
        entity->GetTransform()->SetLocalScale(Vec3(0.25f, 0.25f, 0.25f));
        collider = entity->AddComponent<BoxCollider>();
        collider->SetSize(Vec3(1.0f, 1.0f, 0.01f));
        renderer = entity->AddComponent<MeshRenderer>();
        renderer->SetMesh(position_plane_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_x_axis_color);

        entity = Entity::Create("Gizmo_Position_XYZ", Vec3::Zero(), Quaternion::Identity(), m_gizmo_position_full->GetTransform());
        entity->AddTag(POSITION_XYZ_TAG);
        entity->GetTransform()->SetLocalScale(Vec3(0.25f, 0.25f, 0.25f));
        entity->AddComponent<SphereCollider>();
        renderer = entity->AddComponent<MeshRenderer>();
        renderer->SetMesh(AssetManager::GetMeshPrimitive(MeshPrimitive::Sphere));
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_xyz_axis_color);

        entity = Entity::Create("Gizmo_Rotation_X", Vec3::Zero(), Quaternion::Identity(), m_gizmo_rotation->GetTransform());
        entity->AddTag(ROTATION_X_TAG);
        entity->GetTransform()->SetEulerAngles(Vec3(0.0f, 0.0f, 90.0f));
        collider = entity->AddComponent<BoxCollider>();
        collider->SetSize(Vec3(0.8f, 0.01f, 0.8f));
        collider->SetOrigin(Vec3(0.4f, 0.0f, 0.4f));
        renderer = entity->AddComponent<MeshRenderer>();
        renderer->SetMesh(rotation_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_x_axis_color);

        entity = Entity::Create("Gizmo_Rotation_Y", Vec3::Zero(), Quaternion::Identity(), m_gizmo_rotation->GetTransform());
        entity->AddTag(ROTATION_Y_TAG);
        collider = entity->AddComponent<BoxCollider>();
        collider->SetSize(Vec3(0.8f, 0.01f, 0.8f));
        collider->SetOrigin(Vec3(0.4f, 0.0f, 0.4f));
        renderer = entity->AddComponent<MeshRenderer>();
        renderer->SetMesh(rotation_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_y_axis_color);

        entity = Entity::Create("Gizmo_Rotation_Z", Vec3::Zero(), Quaternion::Identity(), m_gizmo_rotation->GetTransform());
        entity->AddTag(ROTATION_Z_TAG);
        entity->GetTransform()->SetEulerAngles(Vec3(-90.0f, 0.0f, 0.0f));
        collider = entity->AddComponent<BoxCollider>();
        collider->SetSize(Vec3(0.8f, 0.01f, 0.8f));
        collider->SetOrigin(Vec3(0.4f, 0.0f, 0.4f));
        renderer = entity->AddComponent<MeshRenderer>();
        renderer->SetMesh(rotation_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_z_axis_color);

        entity = Entity::Create("Gizmo_Scale_X", Vec3::Zero(), Quaternion::Identity(), m_gizmo_scale->GetTransform());
        entity->AddTag(SCALE_X_TAG);
        entity->GetTransform()->SetEulerAngles(Vec3(0.0f, 0.0f, -90.0f));
        collider = entity->AddComponent<BoxCollider>();
        collider->SetOrigin(Vec3(0.0f, 0.9f, 0.0f));
        collider->SetSize(Vec3(0.2f, 0.2f, 0.2f));
        renderer = entity->AddComponent<MeshRenderer>();
        renderer->SetMesh(scale_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_x_axis_color);

        entity = Entity::Create("Gizmo_Scale_Y", Vec3::Zero(), Quaternion::Identity(), m_gizmo_scale->GetTransform());
        entity->AddTag(SCALE_Y_TAG);
        collider = entity->AddComponent<BoxCollider>();
        collider->SetOrigin(Vec3(0.0f, 0.9f, 0.0f));
        collider->SetSize(Vec3(0.2f, 0.2f, 0.2f));
        renderer = entity->AddComponent<MeshRenderer>();
        renderer->SetMesh(scale_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_y_axis_color);

        entity = Entity::Create("Gizmo_Scale_Z", Vec3::Zero(), Quaternion::Identity(), m_gizmo_scale->GetTransform());
        entity->AddTag(SCALE_Z_TAG);
        entity->GetTransform()->SetEulerAngles(Vec3(90.0f, 0.0f, 0.0f));
        collider = entity->AddComponent<BoxCollider>();
        collider->SetOrigin(Vec3(0.0f, 0.9f, 0.0f));
        collider->SetSize(Vec3(0.2f, 0.2f, 0.2f));
        renderer = entity->AddComponent<MeshRenderer>();
        renderer->SetMesh(scale_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_z_axis_color);

        entity = Entity::Create("Gizmo_Scale_XYZ", Vec3::Zero(), Quaternion::Identity(), m_gizmo_scale->GetTransform());
        entity->AddTag(SCALE_XYZ_TAG);
        entity->GetTransform()->SetLocalScale(Vec3(0.2f, 0.2f, 0.2f));
        collider = entity->AddComponent<BoxCollider>();
        renderer = entity->AddComponent<MeshRenderer>();
        renderer->SetMesh(AssetManager::GetMeshPrimitive(MeshPrimitive::Cube));
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_xyz_axis_color);

        EditorSelection::RegisterSelectionListener(this);
        SetGizmoType(GizmoType::Position);
    }

    void EditorGizmo::OnUpdate(f32 delta_time) {
        if (m_selection == nullptr) {
            return;
        }

        Transform *transform = GetTransform();
        f32 hit_distance = 0.0f;

        Vec3 position = m_selection->GetTransform()->GetPosition();
        Quaternion rotation = m_selection->GetTransform()->GetRotation();
        Vec3 scale = m_selection->GetTransform()->GetLocalScale();

        Plane xy_plane = Plane(Vec3::Forward(), position.z);
        Plane xz_plane = Plane(Vec3::Up(), -position.y);
        Plane yz_plane = Plane(Vec3::Right(), -position.x);

        if (Input::GetMouseButtonUp(MouseButtonCode::Left)) {
            m_gizmo_mode = GizmoMode::None;

            m_position_offset = Vec3::Zero();
        }

        UpdateScale();

        Ray ray = m_camera->ScreenPointToRay(Input::GetMousePosition());
        Physics::RaycastResult result;
        if (GetWorld()->GetPhysicsWorld()->Raycast(ray, result, 1000)) {
            Entity *entity = result.collider->GetEntity();
            MeshRenderer *renderer = entity->GetComponent<MeshRenderer>();

            if (m_gizmo_mode == GizmoMode::None) {
                if (m_last_gizmo != nullptr && m_last_gizmo != entity) {
                    ResetColor();
                }

                result.collider->GetEntity()->GetComponent<MeshRenderer>()->GetMaterial()->SetColor("u_color", m_highlight_color);

                m_last_gizmo = entity;
            }

            if (Input::GetMouseButtonDown(MouseButtonCode::Left)) {
                SetGizmoMode(entity, position);

                switch (m_gizmo_mode) {
                    case GizmoMode::PositionXAxis: {
                        xz_plane.Intersects(ray, hit_distance);
                        m_position_offset.x = position.x - ray.GetPoint(hit_distance).x;
                        break;
                    }
                    case GizmoMode::PositionYAxis: {
                        xy_plane.Intersects(ray, hit_distance);
                        m_position_offset.y = position.y - ray.GetPoint(hit_distance).y;
                        break;
                    }
                    case GizmoMode::PositionZAxis: {
                        xz_plane.Intersects(ray, hit_distance);
                        m_position_offset.z = position.z - ray.GetPoint(hit_distance).z;
                        break;
                    }
                    case GizmoMode::PositionXYAxis: {
                        xy_plane.Intersects(ray, hit_distance);
                        Vec3 hit = ray.GetPoint(hit_distance);
                        m_position_offset.x = position.x - hit.x;
                        m_position_offset.y = position.y - hit.y;
                        break;
                    }
                    case GizmoMode::PositionXZAxis: {
                        xz_plane.Intersects(ray, hit_distance);
                        Vec3 hit = ray.GetPoint(hit_distance);
                        m_position_offset.x = position.x - hit.x;
                        m_position_offset.z = position.z - hit.z;
                        break;
                    }
                    case GizmoMode::PositionYZAxis: {
                        yz_plane.Intersects(ray, hit_distance);
                        Vec3 hit = ray.GetPoint(hit_distance);
                        m_position_offset.y = position.y - hit.y;
                        m_position_offset.z = position.z - hit.z;
                        break;
                    }
                    case GizmoMode::PositionXYZAxis: {
                        m_grabbing_plane.Intersects(ray, hit_distance);
                        Vec3 hit = ray.GetPoint(hit_distance);
                        m_position_offset = position - hit;
                        break;
                    }
                    case GizmoMode::RotationXAxis: {
                        Vec2 screen_point = m_camera->WorldToScreenPoint(position);
                        Vec2 diff = screen_point - Input::GetMousePosition();
                        m_rotation_offset = diff;
                        break;
                    }
                    case GizmoMode::RotationYAxis: {
                        Vec2 screen_point = m_camera->WorldToScreenPoint(position);
                        Vec2 diff = screen_point - Input::GetMousePosition();
                        m_rotation_offset = diff;
                        break;
                    }
                    case GizmoMode::RotationZAxis: {
                        Vec2 screen_point = m_camera->WorldToScreenPoint(position);
                        Vec2 diff = screen_point - Input::GetMousePosition();
                        m_rotation_offset = diff;
                        break;
                    }
                    case GizmoMode::ScaleXAxis: {
                        xz_plane.Intersects(ray, hit_distance);
                        m_scale_offset.x = ray.GetPoint(hit_distance).x;
                        break;
                    }
                    case GizmoMode::ScaleYAxis: {
                        xy_plane.Intersects(ray, hit_distance);
                        m_scale_offset.y = ray.GetPoint(hit_distance).y;
                        break;
                    }
                    case GizmoMode::ScaleZAxis: {
                        xz_plane.Intersects(ray, hit_distance);
                        m_scale_offset.z = ray.GetPoint(hit_distance).z;
                        break;
                    }
                    case GizmoMode::ScaleXYZAxis: {
                        m_grabbing_plane.Intersects(ray, hit_distance);
                        m_scale_offset = ray.GetPoint(hit_distance);
                        break;
                    }
                }
            }
        } else {
            if (m_last_gizmo != nullptr && m_gizmo_mode == GizmoMode::None) {
                ResetColor();
                m_last_gizmo = nullptr;
            }
        }

        bool snapping = Input::GetKey(KeyCode::LeftControl);
        switch (m_gizmo_mode) {
            case GizmoMode::PositionXAxis: {
                xz_plane.Intersects(ray, hit_distance);
                f32 x = ray.GetPoint(hit_distance).x + m_position_offset.x;
                if (snapping) {
                    x = Math::Round(x);
                }
                position.x = x;
                break;
            }
            case GizmoMode::PositionYAxis: {
                xy_plane.Intersects(ray, hit_distance);
                f32 y = ray.GetPoint(hit_distance).y + m_position_offset.y;
                if (snapping) {
                    y = Math::Round(y);
                }
                position.y = y;
                break;
            }
            case GizmoMode::PositionZAxis: {
                xz_plane.Intersects(ray, hit_distance);
                f32 z = ray.GetPoint(hit_distance).z + m_position_offset.z;
                if (snapping) {
                    z = Math::Round(z);
                }
                position.z = z;
                break;
            }
            case GizmoMode::PositionXYAxis: {
                xy_plane.Intersects(ray, hit_distance);
                Vec3 hit = ray.GetPoint(hit_distance);

                f32 x = hit.x + m_position_offset.x;
                f32 y = hit.y + m_position_offset.y;

                if (snapping) {
                    x = Math::Round(x);
                    y = Math::Round(y);
                }

                position.x = x;
                position.y = y;
                break;
            }
            case GizmoMode::PositionXZAxis: {
                xz_plane.Intersects(ray, hit_distance);
                Vec3 hit = ray.GetPoint(hit_distance);

                f32 x = hit.x + m_position_offset.x;
                f32 z = hit.z + m_position_offset.z;

                if (snapping) {
                    x = Math::Round(x);
                    z = Math::Round(z);
                }

                position.x = x;
                position.z = z;
                break;
            }
            case GizmoMode::PositionYZAxis: {
                yz_plane.Intersects(ray, hit_distance);
                Vec3 hit = ray.GetPoint(hit_distance);

                f32 y = hit.y + m_position_offset.y;
                f32 z = hit.z + m_position_offset.z;

                if (snapping) {
                    y = Math::Round(y);
                    z = Math::Round(z);
                }

                position.y = y;
                position.z = z;
                break;
            }
            case GizmoMode::PositionXYZAxis: {
                m_grabbing_plane.Intersects(ray, hit_distance);
                Vec3 hit = ray.GetPoint(hit_distance);
                position = hit + m_position_offset;
                break;
            }
            case GizmoMode::RotationXAxis: {
                Vec2 screen_point = m_camera->WorldToScreenPoint(position);
                Vec2 diff = screen_point - Input::GetMousePosition();

                f32 x = diff.x - m_rotation_offset.x;
                f32 rot = x * m_rotation_speed;
                
                if (snapping) {
                    rot = Math::Round(rot / m_rotation_snap) * m_rotation_snap;
                }

                Quaternion r = Quaternion::FromAxisAngle(Vec3::Right(), rot);

                rotation = r * m_rotation_start;
                break;
            }
            case GizmoMode::RotationYAxis: {
                Vec2 screen_point = m_camera->WorldToScreenPoint(position);
                Vec2 diff = screen_point - Input::GetMousePosition();

                f32 y = diff.x - m_rotation_offset.x;
                f32 rot = y * m_rotation_speed;

                if (snapping) {
                    rot = Math::Round(rot / m_rotation_snap) * m_rotation_snap;
                }

                Quaternion r = Quaternion::FromAxisAngle(Vec3::Down(), rot);

                rotation = r * m_rotation_start;
                break;
            }
            case GizmoMode::RotationZAxis: {
                Vec2 screen_point = m_camera->WorldToScreenPoint(position);
                Vec2 diff = screen_point - Input::GetMousePosition();

                f32 z = diff.x - m_rotation_offset.x;
                f32 rot = z * m_rotation_speed;

                if (snapping) {
                    rot = Math::Round(rot / m_rotation_snap) * m_rotation_snap;
                }

                Quaternion r = Quaternion::FromAxisAngle(Vec3::Back(), rot);

                rotation = r * m_rotation_start;
                break;
            }
            case GizmoMode::ScaleXAxis: {
                xz_plane.Intersects(ray, hit_distance);
                Vec3 hit = ray.GetPoint(hit_distance);

                f32 diff = hit.x - m_scale_offset.x;
                scale.x = m_scale_start.x + diff;
                break;
            }
            case GizmoMode::ScaleYAxis: {
                xy_plane.Intersects(ray, hit_distance);
                Vec3 hit = ray.GetPoint(hit_distance);

                f32 diff = hit.y - m_scale_offset.y;
                scale.y = m_scale_start.y + diff;
                break;
            }
            case GizmoMode::ScaleZAxis: {
                xz_plane.Intersects(ray, hit_distance);
                Vec3 hit = ray.GetPoint(hit_distance);

                f32 diff = hit.z - m_scale_offset.z;
                scale.z = m_scale_start.z + diff;
                break;
            }
            case GizmoMode::ScaleXYZAxis: {
                m_grabbing_plane.Intersects(ray, hit_distance);
                Vec3 hit = ray.GetPoint(hit_distance);

                f32 diff = hit.x - m_scale_offset.x;
                diff += hit.y - m_scale_offset.y;

                scale.x = m_scale_start.x + diff;
                scale.y = m_scale_start.y + diff;
                scale.z = m_scale_start.z + diff;
                break;
            }
        }

        GetTransform()->SetPosition(position);

        m_selection->GetTransform()->SetPosition(position);
        m_selection->GetTransform()->SetRotation(rotation);
        m_selection->GetTransform()->SetLocalScale(scale);
    }

    void EditorGizmo::SetGizmoMode(Entity *entity, Vec3 position) {
        if (entity->HasTag(POSITION_X_TAG)) {
            m_gizmo_mode = GizmoMode::PositionXAxis;
        } else if (entity->HasTag(POSITION_Y_TAG)) {
            m_gizmo_mode = GizmoMode::PositionYAxis;
        } else if (entity->HasTag(POSITION_Z_TAG)) {
            m_gizmo_mode = GizmoMode::PositionZAxis;
        } else if (entity->HasTag(POSITION_XY_TAG)) {
            m_gizmo_mode = GizmoMode::PositionXYAxis;
        } else if (entity->HasTag(POSITION_XZ_TAG)) {
            m_gizmo_mode = GizmoMode::PositionXZAxis;
        } else if (entity->HasTag(POSITION_YZ_TAG)) {
            m_gizmo_mode = GizmoMode::PositionYZAxis;
        } else if (entity->HasTag(POSITION_XYZ_TAG)) {
            m_gizmo_mode = GizmoMode::PositionXYZAxis;
        } else if (entity->HasTag(ROTATION_X_TAG)) {
            m_gizmo_mode = GizmoMode::RotationXAxis;
        } else if (entity->HasTag(ROTATION_Y_TAG)) {
            m_gizmo_mode = GizmoMode::RotationYAxis;
        } else if (entity->HasTag(ROTATION_Z_TAG)) {
            m_gizmo_mode = GizmoMode::RotationZAxis;
        } else if (entity->HasTag(SCALE_X_TAG)) {
            m_gizmo_mode = GizmoMode::ScaleXAxis;
        } else if (entity->HasTag(SCALE_Y_TAG)) {
            m_gizmo_mode = GizmoMode::ScaleYAxis;
        } else if (entity->HasTag(SCALE_Z_TAG)) {
            m_gizmo_mode = GizmoMode::ScaleZAxis;
        } else if (entity->HasTag(SCALE_XYZ_TAG)) {
            m_gizmo_mode = GizmoMode::ScaleXYZAxis;
        }

        m_rotation_start_angles = m_selection->GetTransform()->GetEulerAngles();
        m_rotation_start = m_selection->GetTransform()->GetRotation();
        m_scale_start = m_selection->GetTransform()->GetLocalScale();
        m_grabbing_plane = Plane(m_camera->GetTransform()->GetForward(), position);
    }

    void EditorGizmo::ResetColor() {
        Color color;
        if (m_last_gizmo->HasTag(POSITION_X_TAG)) {
            color = m_x_axis_color;
        } else if (m_last_gizmo->HasTag(POSITION_Y_TAG)) {
            color = m_y_axis_color;
        } else if (m_last_gizmo->HasTag(POSITION_Z_TAG)) {
            color = m_z_axis_color;
        } else if (m_last_gizmo->HasTag(POSITION_XY_TAG)) {
            color = m_z_axis_color;
        } else if (m_last_gizmo->HasTag(POSITION_XZ_TAG)) {
            color = m_y_axis_color;
        } else if (m_last_gizmo->HasTag(POSITION_YZ_TAG)) {
            color = m_x_axis_color;
        } else if (m_last_gizmo->HasTag(POSITION_XYZ_TAG)) {
            color = m_xyz_axis_color;
        } else if (m_last_gizmo->HasTag(ROTATION_X_TAG)) {
            color = m_x_axis_color;
        } else if (m_last_gizmo->HasTag(ROTATION_Y_TAG)) {
            color = m_y_axis_color;
        } else if (m_last_gizmo->HasTag(ROTATION_Z_TAG)) {
            color = m_z_axis_color;
        } else if (m_last_gizmo->HasTag(SCALE_X_TAG)) {
            color = m_x_axis_color;
        } else if (m_last_gizmo->HasTag(SCALE_Y_TAG)) {
            color = m_y_axis_color;
        } else if (m_last_gizmo->HasTag(SCALE_Z_TAG)) {
            color = m_z_axis_color;
        } else if (m_last_gizmo->HasTag(SCALE_XYZ_TAG)) {
            color = m_xyz_axis_color;
        }
        m_last_gizmo->GetComponent<MeshRenderer>()->GetMaterial()->SetColor("u_color", color);
    }

    void EditorGizmo::UpdateScale() {
        Plane camera_plane = Plane(m_camera->GetTransform()->GetForward(), m_camera->GetTransform()->GetPosition());

        f32 scale_factor = camera_plane.GetDistanceToPoint(GetTransform()->GetPosition()) * m_camera->GetFOV() * m_gizmo_scale_factor;
        Vec3 scale = Vec3(scale_factor, scale_factor, scale_factor);
        GetTransform()->SetLocalScale(scale);
    }

    void EditorGizmo::OnSelection(Object *selection) {
        if (selection != nullptr && selection->GetType() == rttr::type::get<Entity>()) {
            Entity *entity = static_cast<Entity *>(selection);
            m_selection = entity;
            GetEntity()->SetActive(true);
            GetTransform()->SetPosition(entity->GetTransform()->GetPosition());

            UpdateScale();
        } else {
            m_selection = nullptr;
            GetEntity()->SetActive(false);
        }
    }

}
