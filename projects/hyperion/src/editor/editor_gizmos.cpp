#include "hyppch.hpp"

#include "hyperion/editor/editor_gizmos.hpp"

#include "hyperion/rendering/forward_renderer.hpp"
#include "hyperion/assets/asset_manager.hpp"
#include "hyperion/assets/mesh_factory.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion::Editor {

    void EditorGizmos::Init() {
        m_material = Material::Create(AssetManager::GetShader("standard_unlit"));
        m_mesh = MeshFactory::CreateFromFile("data/models/gizmo.obj");
    }

    void EditorGizmos::Render(const CameraData &camera) {
        ForwardRenderer::Begin(camera);
        {
            // Scale the gizmos in a way so that they appear at same size
            // no matter the distance and fov of the camera
            f32 distance = camera.position.Magnitude() * camera.fov * m_gizmo_scale;
            Mat4 scale = Mat4::Scale(Vec3(distance, distance, distance));

            // X gizmo
            m_material->SetVec4("u_color", Color::Red());
            ForwardRenderer::DrawMesh(m_mesh, m_material, scale * Mat4::Rotate(Vec3(0, 0, 1), -90.0f));
            // Y gizmo
            m_material->SetVec4("u_color", Color::Green());
            ForwardRenderer::DrawMesh(m_mesh, m_material, scale * Mat4::Identity());
            // Z gizmo
            m_material->SetVec4("u_color", Color::Blue());
            ForwardRenderer::DrawMesh(m_mesh, m_material, scale * Mat4::Rotate(Vec3(1, 0, 0), 90.0f));
        }
        ForwardRenderer::End();
    }

}