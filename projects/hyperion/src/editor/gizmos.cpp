#include "hyppch.hpp"

#include "hyperion/editor/gizmos.hpp"

#include "hyperion/rendering/renderer.hpp"
#include "hyperion/assets/asset_library.hpp"

using namespace Hyperion::Rendering;

namespace Hyperion::Editor {

    void Gizmos::Init() {
        m_shader = AssetLibrary::GetShader("gizmo");
        m_mesh = Mesh::CreateFromFile("data/models/gizmo.obj");

        m_x_axis_aabb = AABB(Vec3(0.5f, 0.0f, 0.0f), Vec3(1.0f, 0.1f, 0.1f));
        m_y_axis_aabb = AABB(Vec3(0.0f, 0.5f, 0.0f), Vec3(0.1f, 1.0f, 0.1f));
        m_z_axis_aabb = AABB(Vec3(0.0f, 0.0f, 0.5f), Vec3(0.1f, 0.1f, 1.0f));
    }

    void Gizmos::Update(const Ref<Camera> &camera) {
        Vec2 mouse_position = Input::GetMousePosition();
        Ray ray = camera->ScreenPointToRay(mouse_position);
        m_gizmo_x_hit = m_x_axis_aabb.Intersects(ray);
        m_gizmo_y_hit = m_y_axis_aabb.Intersects(ray);
        m_gizmo_z_hit = m_z_axis_aabb.Intersects(ray);
    }

    void Gizmos::Render(const Ref<Camera> &camera) {
        Renderer::Begin(camera);
        {
            // We draw the gizmos on top of everything so erase depth information
            RenderEngine::Clear(ClearMask::Depth);

            m_shader->Bind();
            m_shader->SetFloat3("u_camera.position", camera->GetPosition());

            // X gizmo
            m_shader->SetFloat3("u_color", m_gizmo_x_hit ? Vec3(1, 1, 1) : Vec3(1, 0, 0));
            Renderer::Submit(m_mesh, m_shader, Mat4::Rotate(Vec3(0, 0, 1), -90.0f));
            // Y gizmo
            m_shader->SetFloat3("u_color", m_gizmo_y_hit ? Vec3(1, 1, 1) : Vec3(0, 1, 0));
            Renderer::Submit(m_mesh, m_shader, Mat4::Identity());
            // Z gizmo
            m_shader->SetFloat3("u_color", m_gizmo_z_hit ? Vec3(1, 1, 1) : Vec3(0, 0, 1));
            Renderer::Submit(m_mesh, m_shader, Mat4::Rotate(Vec3(1, 0, 0), 90.0f));
        }
        Renderer::End();
    }

}