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
        Mesh *rotation_mesh = AssetManager::GetMeshPrimitive(MeshPrimitive::Quad);
        Mesh *scale_mesh = MeshFactory::CreateFromFile("data/models/gizmo/gizmo_scale.obj");

        m_gizmo_position_axis = Entity::Create("Gizmo_Position_Axis", Vec3::Zero(), Quaternion::Identity(), GetTransform());
        m_gizmo_position_plane = Entity::Create("Gizmo_Position_Plane", Vec3::Zero(), Quaternion::Identity(), GetTransform());
        m_gizmo_position_full = Entity::Create("Gizmo_Position_Full", Vec3::Zero(), Quaternion::Identity(), GetTransform());
        m_gizmo_rotation = Entity::Create("Gizmo_Rotation", Vec3::Zero(), Quaternion::Identity(), GetTransform());
        m_gizmo_scale = Entity::Create("Gizmo_Scale", Vec3::Zero(), Quaternion::Identity(), GetTransform());
        m_gizmo_scale_full = Entity::Create("Gizmo_Scale_Full", Vec3::Zero(), Quaternion::Identity(), GetTransform());

        m_gimzo_position_x = Entity::Create("Gizmo_Position_X", Vec3::Zero(), Quaternion::Identity(), m_gizmo_position_axis->GetTransform());
        m_gimzo_position_x->AddTag(POSITION_X_TAG);
        m_gimzo_position_x->GetTransform()->SetEulerAngles(Vec3(0.0f, 0.0f, -90.0));
        BoxCollider *collider = m_gimzo_position_x->AddComponent<BoxCollider>();
        collider->SetOrigin(Vec3(0.0f, 0.5f, 0.0f));
        collider->SetSize(Vec3(0.1f, 1.0f, 0.1f));
        MeshRenderer *renderer = m_gimzo_position_x->AddComponent<MeshRenderer>();
        renderer->SetMesh(position_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_x_axis_color);

        m_gimzo_position_y = Entity::Create("Gizmo_Position_Y", Vec3::Zero(), Quaternion::Identity(), m_gizmo_position_axis->GetTransform());
        m_gimzo_position_y->AddTag(POSITION_Y_TAG);
        collider = m_gimzo_position_y->AddComponent<BoxCollider>();
        collider->SetOrigin(Vec3(0.0f, 0.5f, 0.0f));
        collider->SetSize(Vec3(0.1f, 1.0f, 0.1f));
        renderer = m_gimzo_position_y->AddComponent<MeshRenderer>();
        renderer->SetMesh(position_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_y_axis_color);
        
        m_gimzo_position_z = Entity::Create("Gizmo_Position_Z", Vec3::Zero(), Quaternion::Identity(), m_gizmo_position_axis->GetTransform());
        m_gimzo_position_z->AddTag(POSITION_Z_TAG);
        m_gimzo_position_z->GetTransform()->SetEulerAngles(Vec3(90.0f, 0.0f, 0.0f));
        collider = m_gimzo_position_z->AddComponent<BoxCollider>();
        collider->SetOrigin(Vec3(0.0f, 0.5f, 0.0f));
        collider->SetSize(Vec3(0.1f, 1.0f, 0.1f));
        renderer = m_gimzo_position_z->AddComponent<MeshRenderer>();
        renderer->SetMesh(position_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_z_axis_color);

        m_gizmo_position_xy = Entity::Create("Gizmo_Position_XY", Vec3::Zero(), Quaternion::Identity(), m_gizmo_position_plane->GetTransform());
        m_gizmo_position_xy->AddTag(POSITION_XY_TAG);
        m_gizmo_position_xy->GetTransform()->SetEulerAngles(Vec3(0.0f, 0.0f, 0.0f));
        m_gizmo_position_xy->GetTransform()->SetPosition(Vec3(0.5f, 0.5f, 0.0f));
        m_gizmo_position_xy->GetTransform()->SetLocalScale(Vec3(0.25f, 0.25f, 0.25f));
        collider = m_gizmo_position_xy->AddComponent<BoxCollider>();
        collider->SetSize(Vec3(1.0f, 1.0f, 0.01f));
        renderer = m_gizmo_position_xy->AddComponent<MeshRenderer>();
        renderer->SetMesh(position_plane_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_z_axis_color);

        m_gizmo_position_xz = Entity::Create("Gizmo_Position_XZ", Vec3::Zero(), Quaternion::Identity(), m_gizmo_position_plane->GetTransform());
        m_gizmo_position_xz->AddTag(POSITION_XZ_TAG);
        m_gizmo_position_xz->GetTransform()->SetEulerAngles(Vec3(90.0f, 0.0f, 0.0f));
        m_gizmo_position_xz->GetTransform()->SetPosition(Vec3(0.5f, 0.0f, 0.5f));
        m_gizmo_position_xz->GetTransform()->SetLocalScale(Vec3(0.25f, 0.25f, 0.25f));
        collider = m_gizmo_position_xz->AddComponent<BoxCollider>();
        collider->SetSize(Vec3(1.0f, 1.0f, 0.01f));
        renderer = m_gizmo_position_xz->AddComponent<MeshRenderer>();
        renderer->SetMesh(position_plane_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_y_axis_color);

        m_gizmo_position_yz = Entity::Create("Gizmo_Position_YZ", Vec3::Zero(), Quaternion::Identity(), m_gizmo_position_plane->GetTransform());
        m_gizmo_position_yz->AddTag(POSITION_YZ_TAG);
        m_gizmo_position_yz->GetTransform()->SetEulerAngles(Vec3(0.0f, 90.0f, 0.0f));
        m_gizmo_position_yz->GetTransform()->SetPosition(Vec3(0.0f, 0.5f, 0.5f));
        m_gizmo_position_yz->GetTransform()->SetLocalScale(Vec3(0.25f, 0.25f, 0.25f));
        collider = m_gizmo_position_yz->AddComponent<BoxCollider>();
        collider->SetSize(Vec3(1.0f, 1.0f, 0.01f));
        renderer = m_gizmo_position_yz->AddComponent<MeshRenderer>();
        renderer->SetMesh(position_plane_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_x_axis_color);

        m_gizmo_position_yz = Entity::Create("Gizmo_Position_XYZ", Vec3::Zero(), Quaternion::Identity(), m_gizmo_position_full->GetTransform());
        m_gizmo_position_yz->AddTag(POSITION_XYZ_TAG);
        m_gizmo_position_yz->GetTransform()->SetLocalScale(Vec3(0.25f, 0.25f, 0.25f));
        m_gizmo_position_yz->AddComponent<SphereCollider>();
        renderer = m_gizmo_position_yz->AddComponent<MeshRenderer>();
        renderer->SetMesh(AssetManager::GetMeshPrimitive(MeshPrimitive::Sphere));
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_xyz_axis_color);

        m_gizmo_rotation_x = Entity::Create("Gizmo_Rotation_X", Vec3::Zero(), Quaternion::Identity(), m_gizmo_rotation->GetTransform());
        m_gizmo_rotation_x->AddTag(ROTATION_X_TAG);
        m_gizmo_rotation_x->GetTransform()->SetEulerAngles(Vec3(0.0f, 90.0f, 0.0f));
        m_gizmo_rotation_x->GetTransform()->SetPosition(Vec3(0.0f, 0.375f, 0.375f));
        m_gizmo_rotation_x->GetTransform()->SetLocalScale(Vec3(0.75f, 0.75f, 0.75f));
        collider = m_gizmo_rotation_x->AddComponent<BoxCollider>();
        collider->SetSize(Vec3(1.0f, 1.0f, 0.01f));
        renderer = m_gizmo_rotation_x->AddComponent<MeshRenderer>();
        renderer->SetMesh(rotation_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_x_axis_color);

        m_gizmo_rotation_y = Entity::Create("Gizmo_Rotation_Y", Vec3::Zero(), Quaternion::Identity(), m_gizmo_rotation->GetTransform());
        m_gizmo_rotation_y->AddTag(ROTATION_Y_TAG);
        m_gizmo_rotation_y->GetTransform()->SetEulerAngles(Vec3(90.0f, 0.0f, 0.0f));
        m_gizmo_rotation_y->GetTransform()->SetPosition(Vec3(0.375f, 0.0f, 0.375f));
        m_gizmo_rotation_y->GetTransform()->SetLocalScale(Vec3(0.75f, 0.75f, 0.75f));
        collider = m_gizmo_rotation_y->AddComponent<BoxCollider>();
        collider->SetSize(Vec3(1.0f, 1.0f, 0.01f));
        renderer = m_gizmo_rotation_y->AddComponent<MeshRenderer>();
        renderer->SetMesh(rotation_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_y_axis_color);

        m_gizmo_rotation_z = Entity::Create("Gizmo_Rotation_Z", Vec3::Zero(), Quaternion::Identity(), m_gizmo_rotation->GetTransform());
        m_gizmo_rotation_z->AddTag(ROTATION_Z_TAG);
        m_gizmo_rotation_z->GetTransform()->SetPosition(Vec3(0.375f, 0.375f, 0.0f));
        m_gizmo_rotation_z->GetTransform()->SetLocalScale(Vec3(0.75f, 0.75f, 0.75f));
        collider = m_gizmo_rotation_z->AddComponent<BoxCollider>();
        collider->SetSize(Vec3(1.0f, 1.0f, 0.01f));
        renderer = m_gizmo_rotation_z->AddComponent<MeshRenderer>();
        renderer->SetMesh(rotation_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_z_axis_color);

        m_gizmo_scale_x = Entity::Create("Gizmo_Scale_X", Vec3::Zero(), Quaternion::Identity(), m_gizmo_scale->GetTransform());
        m_gizmo_scale_x->AddTag(SCALE_X_TAG);
        m_gizmo_scale_x->GetTransform()->SetEulerAngles(Vec3(0.0f, 0.0f, -90.0f));
        collider = m_gizmo_scale_x->AddComponent<BoxCollider>();
        collider->SetOrigin(Vec3(0.0f, 0.9f, 0.0f));
        collider->SetSize(Vec3(0.2f, 0.2f, 0.2f));
        renderer = m_gizmo_scale_x->AddComponent<MeshRenderer>();
        renderer->SetMesh(scale_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_x_axis_color);

        m_gizmo_scale_y = Entity::Create("Gizmo_Scale_Y", Vec3::Zero(), Quaternion::Identity(), m_gizmo_scale->GetTransform());
        m_gizmo_scale_y->AddTag(SCALE_Y_TAG);
        collider = m_gizmo_scale_y->AddComponent<BoxCollider>();
        collider->SetOrigin(Vec3(0.0f, 0.9f, 0.0f));
        collider->SetSize(Vec3(0.2f, 0.2f, 0.2f));
        renderer = m_gizmo_scale_y->AddComponent<MeshRenderer>();
        renderer->SetMesh(scale_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_y_axis_color);

        m_gizmo_scale_z = Entity::Create("Gizmo_Scale_Z", Vec3::Zero(), Quaternion::Identity(), m_gizmo_scale->GetTransform());
        m_gizmo_scale_z->AddTag(SCALE_Z_TAG);
        m_gizmo_scale_z->GetTransform()->SetEulerAngles(Vec3(90.0f, 0.0f, 0.0f));
        collider = m_gizmo_scale_z->AddComponent<BoxCollider>();
        collider->SetOrigin(Vec3(0.0f, 0.9f, 0.0f));
        collider->SetSize(Vec3(0.2f, 0.2f, 0.2f));
        renderer = m_gizmo_scale_z->AddComponent<MeshRenderer>();
        renderer->SetMesh(scale_mesh);
        renderer->SetMaterial(material);
        renderer->GetMaterial()->SetColor("u_color", m_z_axis_color);

        m_gizmo_scale_xyz = Entity::Create("Gizmo_Scale_XYZ", Vec3::Zero(), Quaternion::Identity(), m_gizmo_scale->GetTransform());
        m_gizmo_scale_xyz->AddTag(SCALE_XYZ_TAG);
        m_gizmo_scale_xyz->GetTransform()->SetLocalScale(Vec3(0.2f, 0.2f, 0.2f));
        collider = m_gizmo_scale_xyz->AddComponent<BoxCollider>();
        renderer = m_gizmo_scale_xyz->AddComponent<MeshRenderer>();
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
        Vec3 rotation = m_selection->GetTransform()->GetEulerAngles();
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
                        yz_plane.Intersects(ray, hit_distance);
                        m_rotation_offset.y = ray.GetPoint(hit_distance).y;
                        m_rotation_offset.z = ray.GetPoint(hit_distance).z;
                        break;
                    }
                    case GizmoMode::RotationYAxis: {
                        xz_plane.Intersects(ray, hit_distance);
                        m_rotation_offset.y = ray.GetPoint(hit_distance).y;
                        break;
                    }
                    case GizmoMode::RotationZAxis: {
                        xy_plane.Intersects(ray, hit_distance);
                        m_rotation_offset.z = ray.GetPoint(hit_distance).z;
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
                yz_plane.Intersects(ray, hit_distance);
                Vec3 hit = ray.GetPoint(hit_distance);

                f32 diff = hit.y - m_scale_offset.y;
                diff += hit.z - m_scale_offset.z;

                rotation.x = m_rotation_start.x + diff;
                break;
            }
            case GizmoMode::RotationYAxis: {
                break;
            }
            case GizmoMode::RotationZAxis: {
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
        m_selection->GetTransform()->SetEulerAngles(rotation);
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

        m_rotation_start = m_selection->GetTransform()->GetEulerAngles();
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
        if (selection != nullptr && selection->GetType() == Entity::GetStaticType()) {
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