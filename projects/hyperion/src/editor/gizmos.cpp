#include "hyppch.hpp"

#include "hyperion/editor/gizmos.hpp"

#include "hyperion/rendering/renderer.hpp"
#include "hyperion/assets/asset_library.hpp"
#include "hyperion/assets/mesh_factory.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion::Editor {

    void Gizmos::Init() {
        m_shader = AssetLibrary::GetShader("gizmo");
        m_mesh = MeshFactory::CreateFromFile("data/models/gizmo.obj");
    }

    void Gizmos::Update(const Ref<Camera> &camera) {

    }

    void Gizmos::Render(const Ref<Camera> &camera) {
        Renderer::Begin(camera);
        {
            m_shader->Bind();
            m_shader->SetFloat3("u_camera.position", camera->GetPosition());

            // Scale the gizmos in a way so that they appear at same size
            // no matter the distance and fov of the camera
            f32 fov = camera->GetFOV();
            f32 distance = camera->GetPosition().Magnitude() * fov * m_gizmo_scale;
            Mat4 scale = Mat4::Scale(Vec3(distance, distance, distance));

            // X gizmo
            m_shader->SetFloat3("u_color", Vec3(1, 0, 0));
            Renderer::Submit(m_mesh, m_shader, scale * Mat4::Rotate(Vec3(0, 0, 1), -90.0f));
            // Y gizmo
            m_shader->SetFloat3("u_color", Vec3(0, 1, 0));
            Renderer::Submit(m_mesh, m_shader, scale * Mat4::Identity());
            // Z gizmo
            m_shader->SetFloat3("u_color", Vec3(0, 0, 1));
            Renderer::Submit(m_mesh, m_shader, scale * Mat4::Rotate(Vec3(1, 0, 0), 90.0f));
        }
        Renderer::End();
    }

}